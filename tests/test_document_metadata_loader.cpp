
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

int main(int argc, char*[]) {
    try {
        if (argc != 1) {
            throw std::runtime_error(
                "This test does not require external files"
            );
        }

        dukeai::DocumentMetadataLoader loader;
        TemporaryDirectory temporary;

        // Synthetic public test fixture.
        // It contains no private or copyrighted textbook data.
        const std::string fixture_json = R"({
            "source_id": "dukeai_public_fixture_v1",
            "title": "DukeAI Nexus Public Metadata Loader Fixture",
            "authors": [
                "Example Author One",
                "Example Author Two"
            ],
            "edition": "Test Edition",
            "publication_year": 2026,
            "publisher": "DukeAI Nexus Test Fixtures",
            "document_format": "JSON",
            "page_count": 12,
            "language": "English",
            "subject": [
                "Civic education",
                "Science"
            ],
            "rights_status": "UNVERIFIED",
            "redistribution_allowed": false,
            "public_training_use": false,
            "local_research_use": "TEST_ONLY",
            "extraction_status": "COMPLETED",
            "extracted_words": 2500,
            "review_status": "PENDING"
        })";

        const auto fixture_path =
            temporary.write(
                "public_metadata.json",
                fixture_json
            );

        const auto metadata =
            loader.load_json_file(fixture_path.string());

        check(
            metadata.source_id ==
                "dukeai_public_fixture_v1",
            "synthetic fixture source ID"
        );

        check(
            metadata.title ==
                "DukeAI Nexus Public Metadata Loader Fixture",
            "synthetic fixture title"
        );

        check(
            metadata.authors.size() == 2,
            "author array mapping"
        );

        check(
            metadata.publication_year.has_value() &&
                *metadata.publication_year == 2026,
            "publication year mapping"
        );

        check(
            metadata.page_count.has_value() &&
                *metadata.page_count == 12,
            "page count mapping"
        );

        check(
            metadata.subjects.size() == 2 &&
                metadata.subjects[0] == "Civic education" &&
                metadata.subjects[1] == "Science",
            "singular subject array mapping"
        );

        check(
            metadata.extracted_words.has_value() &&
                *metadata.extracted_words == 2500,
            "extracted word count mapping"
        );

        check(
            metadata.rights_status == "UNVERIFIED" &&
                !metadata.redistribution_allowed &&
                !metadata.public_training_use,
            "rights restrictions preserved"
        );

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
