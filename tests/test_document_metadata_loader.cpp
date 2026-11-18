
#include "dukeai/document_metadata_loader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int passed = 0;

void check(bool condition, const std::string& name) {
    if (!condition) {
        throw std::runtime_error("FAIL: " + name);
    }

    ++passed;
    std::cout << "PASS: " << name << '\n';
}

template <typename Function>
void expect_rejection(
    Function function,
    const std::string& name
) {
    bool rejected = false;

    try {
        function();
    } catch (const std::exception&) {
        rejected = true;
    }

    check(rejected, name);
}

class TemporaryDirectory {
public:
    TemporaryDirectory() {
        path_ =
            std::filesystem::temp_directory_path() /
            "dukeai_metadata_loader_tests";

        std::filesystem::remove_all(path_);
        std::filesystem::create_directories(path_);
    }

    ~TemporaryDirectory() {
        std::error_code error;
        std::filesystem::remove_all(path_, error);
    }

    std::filesystem::path write(
        const std::string& filename,
        const std::string& content
    ) const {
        const auto path = path_ / filename;
        std::ofstream file(path, std::ios::binary);

        if (!file) {
            throw std::runtime_error(
                "Cannot create test fixture"
            );
        }

        file.write(
            content.data(),
            static_cast<std::streamsize>(content.size())
        );

        if (!file) {
            throw std::runtime_error(
                "Cannot write test fixture"
            );
        }

        return path;
    }

private:
    std::filesystem::path path_;
};

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw std::runtime_error(
                "Expected path to textbook metadata JSON"
            );
        }

        dukeai::DocumentMetadataLoader loader;

        const auto metadata =
            loader.load_json_file(argv[1]);

        check(
            metadata.source_id ==
                "tcs_mishra_chandrasekaran_2008",
            "actual metadata source ID"
        );

        check(
            metadata.title ==
                "Theory of Computer Science: Automata, Languages and Computation",
            "actual metadata title"
        );

        check(
            metadata.authors.size() == 2,
            "author array mapping"
        );

        check(
            metadata.publication_year.has_value() &&
                *metadata.publication_year == 2008,
            "publication year mapping"
        );

        check(
            metadata.page_count.has_value() &&
                *metadata.page_count == 434,
            "page count mapping"
        );

        check(
            metadata.subjects.size() == 4,
            "singular subject array mapping"
        );

        check(
            metadata.extracted_words.has_value() &&
                *metadata.extracted_words == 149116,
            "extracted word count mapping"
        );

        check(
            metadata.rights_status == "UNVERIFIED" &&
                !metadata.redistribution_allowed &&
                !metadata.public_training_use,
            "rights restrictions preserved"
        );

        TemporaryDirectory temporary;

        const std::string valid_json = R"({
            "source_id": "test_book",
            "title": "Test Book",
            "language": "English",
            "rights_status": "UNVERIFIED",
            "extraction_status": "COMPLETED",
            "review_status": "PENDING",
            "subject": ["Science"]
        })";

        const auto valid_path =
            temporary.write("valid.json", valid_json);

        const auto valid =
            loader.load_json_file(valid_path.string());

        check(
            valid.subjects.size() == 1 &&
                valid.subjects[0] == "Science",
            "minimal valid metadata"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "malformed.json",
                        R"({"source_id":)"
                    ).string()
                );
            },
            "malformed JSON rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "trailing.json",
                        valid_json + " trailing"
                    ).string()
                );
            },
            "trailing content rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "duplicate.json",
                        R"({
                            "source_id": "first",
                            "source_id": "second",
                            "title": "Test",
                            "language": "English",
                            "rights_status": "UNVERIFIED",
                            "extraction_status": "COMPLETED",
                            "review_status": "PENDING"
                        })"
                    ).string()
                );
            },
            "duplicate keys rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "unknown.json",
                        R"({
                            "source_id": "test",
                            "title": "Test",
                            "language": "English",
                            "rights_status": "UNVERIFIED",
                            "extraction_status": "COMPLETED",
                            "review_status": "PENDING",
                            "unexpected_field": true
                        })"
                    ).string()
                );
            },
            "unknown fields rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "wrong_type.json",
                        R"({
                            "source_id": 123,
                            "title": "Test",
                            "language": "English",
                            "rights_status": "UNVERIFIED",
                            "extraction_status": "COMPLETED",
                            "review_status": "PENDING"
                        })"
                    ).string()
                );
            },
            "wrong field types rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "both_subjects.json",
                        R"({
                            "source_id": "test",
                            "title": "Test",
                            "language": "English",
                            "rights_status": "UNVERIFIED",
                            "extraction_status": "COMPLETED",
                            "review_status": "PENDING",
                            "subject": ["Science"],
                            "subjects": ["Computing"]
                        })"
                    ).string()
                );
            },
            "conflicting subject fields rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "unsafe_rights.json",
                        R"({
                            "source_id": "test",
                            "title": "Test",
                            "language": "English",
                            "rights_status": "UNVERIFIED",
                            "redistribution_allowed": true,
                            "extraction_status": "COMPLETED",
                            "review_status": "PENDING"
                        })"
                    ).string()
                );
            },
            "unverified redistribution permission rejected"
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "bad_array.json",
                        R"({
                            "source_id": "test",
                            "title": "Test",
                            "language": "English",
                            "rights_status": "UNVERIFIED",
                            "extraction_status": "COMPLETED",
                            "review_status": "PENDING",
                            "subject": ["Science", 42]
                        })"
                    ).string()
                );
            },
            "non-string array entries rejected"
        );

        const std::string oversized(
            1024 * 1024 + 1,
            ' '
        );

        expect_rejection(
            [&] {
                loader.load_json_file(
                    temporary.write(
                        "oversized.json",
                        oversized
                    ).string()
                );
            },
            "oversized metadata file rejected"
        );

        std::cout
            << "PASS: all metadata loader tests ("
            << passed << " checks)\n";

        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
