#include "dukeai/retriever.hpp"

#include "dukeai/feature_extractor.hpp"

#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace dukeai {
namespace {

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");

    if (first == std::string::npos) {
        return {};
    }

    const auto last = value.find_last_not_of(" \t\r\n");

    return value.substr(first, last - first + 1);
}

std::vector<std::string> split_paragraphs(
    const std::string& content) {

    std::vector<std::string> paragraphs;
    std::istringstream input(content);
    std::string line;
    std::string current;

    while (std::getline(input, line)) {
        line = trim(line);

        if (line.empty()) {
            if (!current.empty()) {
                paragraphs.push_back(current);
                current.clear();
            }

            continue;
        }

        if (!current.empty()) {
            current += ' ';
        }

        current += line;
    }

    if (!current.empty()) {
        paragraphs.push_back(current);
    }

    return paragraphs;
}

}  // namespace

// The transactional append below relies on Passage being nothrow movable.
static_assert(
    std::is_nothrow_move_constructible<Passage>::value,
    "Passage must be nothrow move constructible");

Retriever::Retriever(const EnergyRanker& ranker)
    : ranker_(ranker) {}

void Retriever::add_document(
    const LoadedDocument& document,
    const DocumentMetadata& metadata) {

    const auto metadata_errors =
        validate_document_metadata(metadata);

    if (!metadata_errors.empty()) {
        throw std::invalid_argument(
            "Invalid document metadata: " + metadata_errors.front());
    }

    const auto match_errors =
        validate_document_match(document.source_id, metadata);

    if (!match_errors.empty()) {
        throw std::invalid_argument(
            "Document metadata mismatch: " + match_errors.front());
    }

    if (document.title != metadata.title) {
        throw std::invalid_argument(
            "Document title does not match metadata title.");
    }

    if (document.language != metadata.language) {
        throw std::invalid_argument(
            "Document language does not match metadata language.");
    }

    // All metadata checks passed. Use the existing transactional path.
    add_document(document);
}

void Retriever::add_document(
    const LoadedDocument& document) {

    add_document_impl(
        document.source_id,
        document.title,
        document.language,
        document.content,
        document.file_path,
        document.file_size_bytes);
}

void Retriever::add_document(
    const std::string& source_id,
    const std::string& title,
    const std::string& language,
    const std::string& content) {

    add_document_impl(
        source_id,
        title,
        language,
        content,
        "",
        0);
}

void Retriever::add_document_impl(
    const std::string& source_id,
    const std::string& title,
    const std::string& language,
    const std::string& content,
    const std::string& file_path,
    std::size_t file_size_bytes) {

    if (trim(source_id).empty()) {
        throw std::invalid_argument("Source ID cannot be empty.");
    }

    if (trim(title).empty()) {
        throw std::invalid_argument("Document title cannot be empty.");
    }

    if (trim(language).empty()) {
        throw std::invalid_argument("Document language cannot be empty.");
    }

    const auto existing = std::find(
        source_ids_.begin(),
        source_ids_.end(),
        source_id);

    if (existing != source_ids_.end()) {
        throw std::invalid_argument(
            "Duplicate source ID: " + source_id);
    }

    // Prepare every passage before modifying the retriever.
    const auto paragraphs = split_paragraphs(content);

    std::vector<Passage> pending;
    pending.reserve(paragraphs.size());

    std::size_t position = 0;

    for (const auto& paragraph : paragraphs) {
        ++position;

        Passage passage;
        passage.passage_id =
            source_id + ":p" + std::to_string(position);
        passage.source_id = source_id;
        passage.title = title;
        passage.language = language;
        passage.file_path = file_path;
        passage.file_size_bytes = file_size_bytes;
        passage.position = position;
        passage.text = paragraph;

        pending.push_back(std::move(passage));
    }

    // Prepare the source ID and reserve capacity before modifying state.
    std::string registered_source_id = source_id;

    source_ids_.reserve(source_ids_.size() + 1);
    passages_.reserve(passages_.size() + pending.size());

    // Both operations below use already-reserved capacity.
    // Moving strings and Passage objects is non-throwing.
    source_ids_.push_back(std::move(registered_source_id));

    passages_.insert(
        passages_.end(),
        std::make_move_iterator(pending.begin()),
        std::make_move_iterator(pending.end()));
}

std::vector<RetrievalResult> Retriever::search(
    const std::string& query,
    const std::string& query_language,
    std::size_t limit) const {

    std::vector<RetrievalResult> results;

    if (limit == 0 || trim(query).empty()) {
        return results;
    }

    for (const auto& passage : passages_) {
        const Features features = extract_features(
            query,
            passage.title,
            passage.text,
            query_language,
            passage.language);

        // Lexical candidate selection: do not return passages
        // that contain none of the query's terms.
        if (features[0] <= 0.0) {
            continue;
        }

        RetrievalResult result;
        result.passage = passage;
        result.features = features;
        result.energy = ranker_.energy(features);

        results.push_back(std::move(result));
    }

    std::stable_sort(
        results.begin(),
        results.end(),
        [](const RetrievalResult& left,
           const RetrievalResult& right) {

            if (left.energy != right.energy) {
                return left.energy < right.energy;
            }

            // Deterministic lexical fallback when energy ties.
            if (left.features[0] != right.features[0]) {
                return left.features[0] > right.features[0];
            }

            if (left.features[1] != right.features[1]) {
                return left.features[1] > right.features[1];
            }

            return left.passage.passage_id <
                   right.passage.passage_id;
        });

    if (results.size() > limit) {
        results.resize(limit);
    }

    return results;
}

std::size_t Retriever::passage_count() const {
    return passages_.size();
}

}  // namespace dukeai
