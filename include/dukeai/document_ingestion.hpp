#pragma once

#include "dukeai/document_loader.hpp"
#include "dukeai/document_metadata_loader.hpp"
#include "dukeai/retriever.hpp"

#include <string>

namespace dukeai {

class DocumentIngestion {
public:
    // Both loaders must outlive this coordinator.
    DocumentIngestion(
        const DocumentLoader& document_loader,
        const DocumentMetadataLoader& metadata_loader);

    // Loads, validates and indexes one document.
    void ingest(
        const std::string& text_file,
        const std::string& metadata_file,
        Retriever& retriever) const;

private:
    const DocumentLoader& document_loader_;
    const DocumentMetadataLoader& metadata_loader_;
};

} // namespace dukeai
