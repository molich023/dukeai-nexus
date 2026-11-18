#include "dukeai/document_loader.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <utility>

namespace dukeai {
namespace {

bool is_blank(const std::string& value) {
    return std::all_of(
        value.begin(),
        value.end(),
        [](unsigned char character) {
            return std::isspace(character) != 0;
        }
    );
}

std::string lowercase_extension(
    const std::filesystem::path& path
) {
    std::string extension = path.extension().string();

    std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        }
    );

    return extension;
}

}  // namespace

DocumentLoader::DocumentLoader(std::size_t max_file_size)
    : max_file_size_(max_file_size) {
    if (max_file_size_ == 0) {
        throw std::invalid_argument(
            "Maximum document size must be greater than zero."
        );
    }
}

std::size_t DocumentLoader::max_file_size() const {
    return max_file_size_;
}

LoadedDocument DocumentLoader::load_text_file(
    const std::string& file_path,
    const std::string& source_id,
    const std::string& title,
    const std::string& language
) const {
    if (file_path.empty()) {
        throw std::invalid_argument("File path must not be empty.");
    }

    if (source_id.empty() || is_blank(source_id)) {
        throw std::invalid_argument("Source ID must not be empty.");
    }

    if (title.empty() || is_blank(title)) {
        throw std::invalid_argument("Document title must not be empty.");
    }

    if (language.empty() || is_blank(language)) {
        throw std::invalid_argument("Language must not be empty.");
    }

    const std::filesystem::path path(file_path);

    if (lowercase_extension(path) != ".txt") {
        throw std::invalid_argument(
            "Unsupported document format: only .txt files are accepted."
        );
    }

    std::error_code error;

    if (!std::filesystem::is_regular_file(path, error)) {
        if (error) {
            throw std::runtime_error(
                "Could not inspect document path: " + error.message()
            );
        }

        throw std::runtime_error(
            "Document path does not refer to a regular file."
        );
    }

    const std::uintmax_t reported_size =
        std::filesystem::file_size(path, error);

    if (error) {
        throw std::runtime_error(
            "Could not determine document size: " + error.message()
        );
    }

    if (reported_size > max_file_size_) {
        throw std::runtime_error(
            "Document exceeds the configured maximum file size."
        );
    }

    std::ifstream input(path, std::ios::binary);

    if (!input.is_open()) {
        throw std::runtime_error("Could not open document for reading.");
    }

    std::string content;
    content.reserve(static_cast<std::size_t>(reported_size));

    char buffer[4096];

    while (true) {
        // If the limit has been reached, check whether another byte exists.
        // This also detects a file that grew after the initial size check.
        if (content.size() == max_file_size_) {
            char extra_byte = '\0';
            input.get(extra_byte);

            if (input) {
                throw std::runtime_error(
                    "Document grew beyond the configured maximum file size."
                );
            }

            if (input.bad() || !input.eof()) {
                throw std::runtime_error(
                    "An error occurred while reading the document."
                );
            }

            break;
        }

        const std::size_t remaining =
            max_file_size_ - content.size();

        const std::size_t bytes_to_read =
            std::min(remaining, sizeof(buffer));

        input.read(
            buffer,
            static_cast<std::streamsize>(bytes_to_read)
        );

        const std::streamsize bytes_read = input.gcount();

        if (bytes_read > 0) {
            content.append(
                buffer,
                static_cast<std::size_t>(bytes_read)
            );
        }

        if (input.bad()) {
            throw std::runtime_error(
                "An I/O error occurred while reading the document."
            );
        }

        if (input.eof()) {
            break;
        }

        if (input.fail()) {
            throw std::runtime_error(
                "Document reading failed unexpectedly."
            );
        }
    }

    if (content.empty() || is_blank(content)) {
        throw std::runtime_error(
            "Document is empty or contains only whitespace."
        );
    }

    LoadedDocument document;
    document.source_id = source_id;
    document.title = title;
    document.language = language;
    document.content = std::move(content);
    document.file_path = path.string();
    document.file_size_bytes = document.content.size();

    return document;
}

}  // namespace dukeai
