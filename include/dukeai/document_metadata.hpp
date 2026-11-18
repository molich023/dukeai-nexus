#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace dukeai {

struct DocumentMetadata {
    std::string source_id;
    std::string title;
    std::vector<std::string> authors;
    std::string edition;

    std::optional<int> publication_year;
    std::string publisher;
    std::string document_format;
    std::optional<std::size_t> page_count;

    std::string language;
    std::vector<std::string> subjects;
    std::string source_url;

    std::string rights_status;
    bool redistribution_allowed = false;
    bool public_training_use = false;
    std::string local_research_use;

    std::string extraction_status;
    std::optional<std::size_t> extracted_words;
    std::string review_status;
};

// Returns every validation problem found.
// An empty vector means the metadata passed validation.
std::vector<std::string> validate_document_metadata(
    const DocumentMetadata& metadata);

// Checks that loaded content and its metadata identify the same source.
std::vector<std::string> validate_document_match(
    const std::string& loaded_source_id,
    const DocumentMetadata& metadata);

} // namespace dukeai
