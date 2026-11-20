
#include "dukeai/document_ingestion.hpp"

#include <filesystem>
#include <stdexcept>

namespace dukeai {
namespace {

std::filesystem::path canonical_directory(
    const std::filesystem::path& path) {

    std::error_code error;
    const auto canonical =
        std::filesystem::canonical(path, error);

    if (error || !std::filesystem::is_directory(canonical)) {
        throw std::runtime_error(
            "Approved ingestion root is not a valid directory");
    }

    return canonical;
}

bool is_within_directory(
    const std::filesystem::path& file,
    const std::filesystem::path& root) {

    auto file_it = file.begin();
    auto root_it = root.begin();

    for (; root_it != root.end(); ++root_it, ++file_it) {
        if (file_it == file.end() || *file_it != *root_it) {
            return false;
        }
    }

    return true;
}

std::filesystem::path validate_input_path(
    const std::filesystem::path& input,
    const std::filesystem::path& root) {

    std::error_code error;
    const auto canonical =
        std::filesystem::canonical(input, error);

    if (error ||
        !std::filesystem::is_regular_file(canonical) ||
        !is_within_directory(canonical, root)) {

        throw std::runtime_error(
            "Input file is invalid or outside the approved root");
    }

    return canonical;
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

    // Validate both input paths before reading either file.
    const auto validated_metadata_path =
        validate_input_path(metadata_file, approved_root_);

    const auto validated_text_path =
        validate_input_path(text_file, approved_root_);

    // Load and validate metadata first.
    const DocumentMetadata metadata =
        metadata_loader_.load_json_file(
            validated_metadata_path.string());

    // Use validated metadata as the source of document identity.
    const LoadedDocument document =
        document_loader_.load_text_file(
            validated_text_path.string(),
            metadata.source_id,
            metadata.title,
            metadata.language);

    // Retriever performs final validation and transactional indexing.
    retriever.add_document(document, metadata);
}

} // namespace dukeai
