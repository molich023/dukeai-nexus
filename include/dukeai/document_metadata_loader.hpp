#pragma once

#include "dukeai/document_metadata.hpp"

#include <string>

namespace dukeai {

class DocumentMetadataLoader {
public:
    DocumentMetadata load_json_file(
        const std::string& file_path) const;
};

} // namespace dukeai
