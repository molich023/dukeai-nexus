#pragma once

#include <cstddef>
#include <string>

namespace dukeai {

struct LoadedDocument {
    std::string source_id;
    std::string title;
    std::string language;
    std::string content;
    std::string file_path;
    std::size_t file_size_bytes;
};

class DocumentLoader {
public:
    static constexpr std::size_t default_max_file_size =
        5 * 1024 * 1024;

    explicit DocumentLoader(
        std::size_t max_file_size = default_max_file_size);

    LoadedDocument load_text_file(
        const std::string& file_path,
        const std::string& source_id,
        const std::string& title,
        const std::string& language) const;

    std::size_t max_file_size() const;

private:
    std::size_t max_file_size_;
};

}  // namespace dukeai
