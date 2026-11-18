#include "dukeai/document_loader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int tests_passed = 0;

void check(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("FAIL: " + message);
    }

    ++tests_passed;
    std::cout << "PASS: " << message << '\n';
}

void write_file(
    const std::filesystem::path& path,
    const std::string& content
) {
    std::ofstream output(path, std::ios::binary);

    if (!output) {
        throw std::runtime_error("Could not create test file.");
    }

    output.write(
        content.data(),
        static_cast<std::streamsize>(content.size())
    );

    if (!output) {
        throw std::runtime_error("Could not write test file.");
    }
}

template <typename Exception, typename Function>
void expect_exception(
    Function function,
    const std::string& description
) {
    try {
        function();
    } catch (const Exception&) {
        check(true, description);
        return;
    }

    throw std::runtime_error(
        "FAIL: expected exception: " + description
    );
}

}  // namespace

int main() {
    namespace fs = std::filesystem;

    const fs::path test_directory =
        fs::temp_directory_path() / "dukeai_document_loader_tests";

    try {
        fs::remove_all(test_directory);
        fs::create_directories(test_directory);

        const fs::path valid_file =
            test_directory / "sample.txt";

        const fs::path uppercase_file =
            test_directory / "sample.TXT";

        const fs::path empty_file =
            test_directory / "empty.txt";

        const fs::path whitespace_file =
            test_directory / "whitespace.txt";

        const fs::path oversized_file =
            test_directory / "oversized.txt";

        const fs::path unsupported_file =
            test_directory / "document.pdf";

        const fs::path directory_path =
            test_directory / "folder.txt";

        const std::string sample =
            "Finite automata are mathematical models of computation.\n"
            "They recognise regular languages.\n";

        write_file(valid_file, sample);
        write_file(uppercase_file, sample);
        write_file(empty_file, "");
        write_file(whitespace_file, " \t\n\r ");
        write_file(oversized_file, "123456789");
        fs::create_directories(directory_path);

        dukeai::DocumentLoader loader;

        check(
            loader.max_file_size() ==
                dukeai::DocumentLoader::default_max_file_size,
            "default maximum file size"
        );

        const auto loaded = loader.load_text_file(
            valid_file.string(),
            "test_source",
            "Finite Automata",
            "en"
        );

        check(loaded.content == sample, "preserves document content");
        check(loaded.source_id == "test_source", "preserves source ID");
        check(loaded.title == "Finite Automata", "preserves title");
        check(loaded.language == "en", "preserves language");
        check(
            loaded.file_size_bytes == sample.size(),
            "reports actual content size"
        );
        check(
            loaded.file_path == valid_file.string(),
            "records supplied file path"
        );

        const auto uppercase_loaded = loader.load_text_file(
            uppercase_file.string(),
            "uppercase_source",
            "Uppercase Extension",
            "en"
        );

        check(
            uppercase_loaded.content == sample,
            "accepts uppercase .TXT extension"
        );

        expect_exception<std::invalid_argument>(
            [&] {
                loader.load_text_file(
                    unsupported_file.string(),
                    "source",
                    "PDF",
                    "en"
                );
            },
            "rejects unsupported file extension"
        );

        expect_exception<std::runtime_error>(
            [&] {
                loader.load_text_file(
                    (test_directory / "missing.txt").string(),
                    "source",
                    "Missing",
                    "en"
                );
            },
            "rejects missing file"
        );

        expect_exception<std::runtime_error>(
            [&] {
                loader.load_text_file(
                    directory_path.string(),
                    "source",
                    "Directory",
                    "en"
                );
            },
            "rejects directory"
        );

        expect_exception<std::runtime_error>(
            [&] {
                loader.load_text_file(
                    empty_file.string(),
                    "source",
                    "Empty",
                    "en"
                );
            },
            "rejects empty file"
        );

        expect_exception<std::runtime_error>(
            [&] {
                loader.load_text_file(
                    whitespace_file.string(),
                    "source",
                    "Whitespace",
                    "en"
                );
            },
            "rejects whitespace-only file"
        );

        dukeai::DocumentLoader small_loader(8);

        const fs::path exact_limit_file =
            test_directory / "exact_limit.txt";

        write_file(exact_limit_file, "12345678");

        const auto exact_limit = small_loader.load_text_file(
            exact_limit_file.string(),
            "limit_source",
            "Exact Limit",
            "en"
        );

        check(
            exact_limit.file_size_bytes == 8,
            "accepts file exactly at size limit"
        );

        expect_exception<std::runtime_error>(
            [&] {
                small_loader.load_text_file(
                    oversized_file.string(),
                    "source",
                    "Oversized",
                    "en"
                );
            },
            "rejects file exceeding size limit"
        );

        expect_exception<std::invalid_argument>(
            [] {
                dukeai::DocumentLoader invalid_loader(0);
            },
            "rejects zero maximum size"
        );

        expect_exception<std::invalid_argument>(
            [&] {
                loader.load_text_file(
                    valid_file.string(),
                    " ",
                    "Title",
                    "en"
                );
            },
            "rejects blank source ID"
        );

        expect_exception<std::invalid_argument>(
            [&] {
                loader.load_text_file(
                    valid_file.string(),
                    "source",
                    " \t ",
                    "en"
                );
            },
            "rejects blank title"
        );

        expect_exception<std::invalid_argument>(
            [&] {
                loader.load_text_file(
                    valid_file.string(),
                    "source",
                    "Title",
                    " "
                );
            },
            "rejects blank language"
        );

        expect_exception<std::invalid_argument>(
            [&] {
                loader.load_text_file(
                    "",
                    "source",
                    "Title",
                    "en"
                );
            },
            "rejects empty file path"
        );

        fs::remove_all(test_directory);

        std::cout << "\nAll " << tests_passed
                  << " document-loader checks passed.\n";

        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        std::cerr << "Test files retained at: "
                  << test_directory << '\n';
        return 1;
    }
}
