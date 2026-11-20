#include "dukeai/document_ingestion.hpp"

namespace dukeai {

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

    // Load and validate metadata first.
    const DocumentMetadata metadata =
        metadata_loader_.load_json_file(metadata_file);

    // Use validated metadata as the source of document identity.
    const LoadedDocument document =
        document_loader_.load_text_file(
            text_file,
            metadata.source_id,
            metadata.title,
            metadata.language);

    // Retriever performs final validation and transactional indexing.
    retriever.add_document(document, metadata);
}

} // namespace dukeai
