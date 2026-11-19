
#pragma once

#include "dukeai/document_loader.hpp"
#include "dukeai/document_metadata_loader.hpp"
#include "dukeai/retriever.hpp"

#include <filesystem>
#include <string>

namespace dukeai {

class DocumentIngestion {
public:
    DocumentIngestion(
        const DocumentLoader& document_loader,
        const DocumentMetadataLoader& metadata_loader,
        const std::filesystem::path& approved_root);

    void ingest(
        const std::string& text_file,
        const std::string& metadata_file,
        Retriever& retriever) const;

private:
    const DocumentLoader& document_loader_;
    const DocumentMetadataLoader& metadata_loader_;
    std::filesystem::path approved_root_;
};

} // namespace dukeai
