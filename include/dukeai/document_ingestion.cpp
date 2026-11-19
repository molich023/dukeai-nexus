
#include "dukeai/document_ingestion.hpp"

#include <filesystem>
#include <stdexcept>
#include <system_error>

namespace dukeai {
namespace {

std::filesystem::path canonical_directory(
    const std::filesystem::path& directory) {

    if (directory.empty()) {
        throw std::invalid_argument(
            "Approved data directory must not be empty.");
    }

    std::error_code error;
    const auto resolved = std::filesystem::canonical(directory, error);

    if (error) {
        throw std::runtime_error(
            "Cannot resolve approved data directory: " +
            error.message());
    }

    if (!std::filesystem::is_directory(resolved, error) || error) {
        throw std::invalid_argument(
            "Approved data path must refer to an existing directory.");
    }

    return resolved;
}

bool is_within_directory(
    const std::filesystem::path& file,
    const std::filesystem::path& directory) {

    auto file_part = file.begin();
    auto directory_part = directory.begin();

    for (; directory_part != directory.end();
         ++directory_part, ++file_part) {

        if (file_part == file.end() ||
            *file_part != *directory_part) {
            return false;
        }
    }

    return true;
}

std::filesystem::path validate_input_path(
    const std::filesystem::path& requested_path,
    const std::filesystem::path& approved_root,
    const char* description) {

    if (requested_path.empty()) {
        throw std::invalid_argument(
            std::string(description) + " path must not be empty.");
    }

    std::error_code error;
    const auto resolved =
        std::filesystem::canonical(requested_path, error);

    if (error) {
        throw std::runtime_error(
            std::string("Cannot resolve ") + description +
            " path: " + error.message());
    }

    if (!is_within_directory(resolved, approved_root)) {
        throw std::runtime_error(
            std::string(description) +
            " path is outside the approved data directory.");
    }

    if (!std::filesystem::is_regular_file(resolved, error) || error) {
        throw std::runtime_error(
            std::string(description) +
            " path must refer to a regular file.");
    }

    return resolved;
}

} // namespace

DocumentIngestion::DocumentIngestion(
    const DocumentLoader& document_loader,
    const DocumentMetadataLoader& metadata_loader,
    const std::filesystem::path& approved_root)
    : document_loader_(document_loader),
      metadata_loader_(metadata_loader),
      approved_root_(canonical_directory(approved_root)) {}

void DocumentIngestion::ingest(
    const std::string& text_file,
    const std::string& metadata_file,
    Retriever& retriever) const {

    // Validate both paths before either loader reads a file.
    const auto safe_metadata_path = validate_input_path(
        metadata_file,
        approved_root_,
        "Metadata");

    const auto safe_text_path = validate_input_path(
        text_file,
        approved_root_,
        "Document");

    // Load and validate metadata first.
    const DocumentMetadata metadata =
        metadata_loader_.load_json_file(safe_metadata_path.string());

    // Use validated metadata as the source of document identity.
    const LoadedDocument document =
        document_loader_.load_text_file(
            safe_text_path.string(),
            metadata.source_id,
            metadata.title,
            metadata.language);

    // Retriever performs final validation and transactional indexing.
    retriever.add_document(document, metadata);
}

} // namespace dukeai
