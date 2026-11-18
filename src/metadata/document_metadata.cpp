#include "dukeai/document_metadata.hpp"

#include <cctype>
#include <string>
#include <vector>

namespace dukeai {
namespace {

bool is_blank(const std::string& value) {
    for (unsigned char character : value) {
        if (!std::isspace(character)) {
            return false;
        }
    }
    return true;
}

void require_text(
    const std::string& value,
    const std::string& field,
    std::vector<std::string>& errors) {

    if (is_blank(value)) {
        errors.push_back(field + " must not be empty or whitespace");
    }
}

} // namespace

std::vector<std::string> validate_document_metadata(
    const DocumentMetadata& metadata) {

    std::vector<std::string> errors;

    require_text(metadata.source_id, "source_id", errors);
    require_text(metadata.title, "title", errors);
    require_text(metadata.language, "language", errors);
    require_text(metadata.rights_status, "rights_status", errors);
    require_text(metadata.review_status, "review_status", errors);

    if (metadata.publication_year.has_value()) {
        if (*metadata.publication_year < 1 ||
            *metadata.publication_year > 9999) {
            errors.push_back(
                "publication_year must be between 1 and 9999");
        }
    }

    if (metadata.page_count.has_value() &&
        *metadata.page_count == 0) {
        errors.push_back("page_count must be greater than zero");
    }

    if (metadata.extraction_status.empty() ||
        is_blank(metadata.extraction_status)) {
        errors.push_back(
            "extraction_status must not be empty or whitespace");
    }

    if (metadata.rights_status == "UNVERIFIED" &&
        (metadata.redistribution_allowed ||
         metadata.public_training_use)) {
        errors.push_back(
            "UNVERIFIED rights cannot permit redistribution "
            "or public training");
    }

    return errors;
}

std::vector<std::string> validate_document_match(
    const std::string& loaded_source_id,
    const DocumentMetadata& metadata) {

    std::vector<std::string> errors;

    if (is_blank(loaded_source_id)) {
        errors.push_back(
            "loaded source ID must not be empty or whitespace");
    } else if (loaded_source_id != metadata.source_id) {
        errors.push_back(
            "loaded source ID does not match metadata source_id");
    }

    return errors;
}

} // namespace dukeai
