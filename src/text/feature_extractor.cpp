#include "dukeai/feature_extractor.hpp"

#include "dukeai/tokenizer.hpp"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>
#include <vector>

namespace dukeai {
namespace {

std::unordered_set<std::string> make_unique_terms(
    const std::vector<std::string>& tokens) {
    return {tokens.begin(), tokens.end()};
}

double overlap_ratio(
    const std::unordered_set<std::string>& query_terms,
    const std::unordered_set<std::string>& target_terms) {

    if (query_terms.empty()) {
        return 0.0;
    }

    std::size_t matches = 0;

    for (const auto& term : query_terms) {
        if (target_terms.count(term) != 0) {
            ++matches;
        }
    }

    return static_cast<double>(matches) /
           static_cast<double>(query_terms.size());
}

std::string normalize_language(const std::string& language) {
    std::string normalized;

    auto first = language.begin();
    auto last = language.end();

    while (first != last &&
           std::isspace(static_cast<unsigned char>(*first))) {
        ++first;
    }

    while (first != last &&
           std::isspace(static_cast<unsigned char>(*(last - 1)))) {
        --last;
    }

    for (auto it = first; it != last; ++it) {
        const unsigned char value =
            static_cast<unsigned char>(*it);

        if (value >= 'A' && value <= 'Z') {
            normalized.push_back(
                static_cast<char>(value - 'A' + 'a'));
        } else {
            normalized.push_back(static_cast<char>(value));
        }
    }

    return normalized;
}

}  // namespace

Features extract_features(
    const std::string& query,
    const std::string& title,
    const std::string& document,
    const std::string& query_language,
    const std::string& document_language) {

    const auto query_terms =
        make_unique_terms(tokenize(query));

    const auto title_terms =
        make_unique_terms(tokenize(title));

    const auto document_terms =
        make_unique_terms(tokenize(document));

    const std::string query_lang =
        normalize_language(query_language);

    const std::string document_lang =
        normalize_language(document_language);

    const double language_match =
        !query_lang.empty() &&
        query_lang == document_lang ? 1.0 : 0.0;

    return Features{
        overlap_ratio(query_terms, document_terms),
        overlap_ratio(query_terms, title_terms),
        language_match
    };
}

}  // namespace dukeai
