
#include "dukeai/document_ingestion.hpp"
#include "dukeai/energy_ranker.hpp"
#include "dukeai/retriever.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int checks = 0;

void check(bool condition, const std::string& message) {
    ++checks;
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void write_file(
    const std::filesystem::path& path,
    const std::string& content) {

    std::ofstream output(path, std::ios::binary);

    if (!output) {
        throw std::runtime_error(
            "Cannot create test file: " + path.string());
    }

    output << content;

    if (!output) {
        throw std::runtime_error(
            "Cannot write test file: " + path.string());
    }
}

class TemporaryDirectory {
public:
    TemporaryDirectory() {
        path_ = std::filesystem::temp_directory_path()
            / "dukeai_document_ingestion_tests";

        std::filesystem::remove_all(path_);
        std::filesystem::create_directories(path_);
    }

    ~TemporaryDirectory() {
        std::error_code error;
        std::filesystem::remove_all(path_, error);
    }

    const std::filesystem::path& path() const {
        return path_;
    }

private:
    std::filesystem::path path_;
};

const std::string valid_metadata = R"({
    "source_id": "ingestion_test_001",
    "title": "Introduction to Automata",
    "authors": ["DukeAI Test Author"],
    "publication_year": 2026,
    "language": "English",
    "subject": ["Computer Science"],
    "rights_status": "UNVERIFIED",
    "redistribution_allowed": false,
    "public_training_use": false,
    "extraction_status": "COMPLETED",
    "review_status": "PENDING"
})";

void test_successful_ingestion(
    const TemporaryDirectory& directory) {

    const auto text_path = directory.path() / "content.txt";
    const auto metadata_path = directory.path() / "metadata.json";

    write_file(
        text_path,
        "Finite automata process input symbols.\n\n"
        "Deterministic finite automata have one transition "
        "for each state and input symbol.");

    write_file(metadata_path, valid_metadata);

    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);
    dukeai::DocumentLoader document_loader;
    dukeai::DocumentMetadataLoader metadata_loader;

    dukeai::DocumentIngestion ingestion(
        document_loader,
        metadata_loader,
        directory.path());

    ingestion.ingest(
        text_path.string(),
        metadata_path.string(),
        retriever);

    check(
        retriever.passage_count() == 2,
        "Successful ingestion should create two passages.");

    const auto results = retriever.search(
        "automata",
        "English",
        10);

    check(!results.empty(),
          "Ingested content should be searchable.");

    for (const auto& result : results) {
        check(
            result.passage.source_id == "ingestion_test_001",
            "Passage source ID should match metadata.");

        check(
            result.passage.title == "Introduction to Automata",
            "Passage title should match metadata.");

        check(
            result.passage.file_path == text_path.string(),
            "Passage should preserve the original text file path.");

        check(
            result.passage.file_size_bytes ==
                std::filesystem::file_size(text_path),
            "Passage should preserve the original file size.");
    }

    std::cout << "PASS: successful ingestion and provenance\n";
}

void test_malformed_metadata(
    const TemporaryDirectory& directory) {

    const auto text_path =
        directory.path() / "malformed_content.txt";

    const auto metadata_path =
        directory.path() / "malformed.json";

    write_file(text_path, "A sample document about computation.");
    write_file(metadata_path, "{ invalid JSON ");

    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);
    dukeai::DocumentLoader document_loader;
    dukeai::DocumentMetadataLoader metadata_loader;

    dukeai::DocumentIngestion ingestion(
        document_loader,
        metadata_loader,
        directory.path());

    bool rejected = false;

    try {
        ingestion.ingest(
            text_path.string(),
            metadata_path.string(),
            retriever);
    } catch (const std::exception&) {
        rejected = true;
    }

    check(rejected, "Malformed metadata must be rejected.");

    check(
        retriever.passage_count() == 0,
        "Malformed metadata must not modify the index.");

    std::cout << "PASS: malformed metadata rejection\n";
}

void test_missing_text_file(
    const TemporaryDirectory& directory) {

    const auto missing_path =
        directory.path() / "missing.txt";

    const auto metadata_path =
        directory.path() / "missing_text.json";

    write_file(metadata_path, valid_metadata);

    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);
    dukeai::DocumentLoader document_loader;
    dukeai::DocumentMetadataLoader metadata_loader;

    dukeai::DocumentIngestion ingestion(
        document_loader,
        metadata_loader,
        directory.path());

    bool rejected = false;

    try {
        ingestion.ingest(
            missing_path.string(),
            metadata_path.string(),
            retriever);
    } catch (const std::exception&) {
        rejected = true;
    }

    check(rejected, "Missing text file must be rejected.");

    check(
        retriever.passage_count() == 0,
        "Missing text file must not modify the index.");

    std::cout << "PASS: missing text file rejection\n";
}

void test_duplicate_ingestion(
    const TemporaryDirectory& directory) {

    const auto text_path =
        directory.path() / "duplicate.txt";

    const auto metadata_path =
        directory.path() / "duplicate.json";

    write_file(
        text_path,
        "A finite automaton recognises regular languages.");

    write_file(metadata_path, valid_metadata);

    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);
    dukeai::DocumentLoader document_loader;
    dukeai::DocumentMetadataLoader metadata_loader;

    dukeai::DocumentIngestion ingestion(
        document_loader,
        metadata_loader,
        directory.path());

    ingestion.ingest(
        text_path.string(),
        metadata_path.string(),
        retriever);

    const std::size_t original_count =
        retriever.passage_count();

    bool rejected = false;

    try {
        ingestion.ingest(
            text_path.string(),
            metadata_path.string(),
            retriever);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }

    check(rejected,
          "Duplicate source ingestion must be rejected.");

    check(
        retriever.passage_count() == original_count,
        "Duplicate ingestion must not change the existing index.");

    std::cout << "PASS: duplicate ingestion rejection\n";
}

void test_outside_root_rejection(
    const TemporaryDirectory& directory) {

    const auto approved_root =
        directory.path() / "approved";

    const auto outside_root =
        directory.path() / "outside";

    std::filesystem::create_directories(approved_root);
    std::filesystem::create_directories(outside_root);

    const auto text_path =
        outside_root / "outside.txt";

    const auto metadata_path =
        approved_root / "metadata.json";

    write_file(text_path, "This file is outside the approved root.");
    write_file(metadata_path, valid_metadata);

    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);
    dukeai::DocumentLoader document_loader;
    dukeai::DocumentMetadataLoader metadata_loader;

    dukeai::DocumentIngestion ingestion(
        document_loader,
        metadata_loader,
        approved_root);

    bool rejected = false;

    try {
        ingestion.ingest(
            text_path.string(),
            metadata_path.string(),
            retriever);
    } catch (const std::exception&) {
        rejected = true;
    }

    check(rejected,
          "Files outside the approved root must be rejected.");

    check(
        retriever.passage_count() == 0,
        "Outside-root rejection must not modify the index.");

    std::cout << "PASS: outside-root rejection\n";
}

void test_symlink_escape_rejection(
    const TemporaryDirectory& directory) {

    const auto approved_root =
        directory.path() / "symlink_approved";

    const auto outside_root =
        directory.path() / "symlink_outside";

    std::filesystem::create_directories(approved_root);
    std::filesystem::create_directories(outside_root);

    const auto outside_text =
        outside_root / "outside.txt";

    const auto metadata_path =
        approved_root / "metadata.json";

    const auto linked_text =
        approved_root / "linked.txt";

    write_file(
        outside_text,
        "This file is reached through a symbolic link.");

    write_file(metadata_path, valid_metadata);

    std::error_code error;

    std::filesystem::create_symlink(
        outside_text,
        linked_text,
        error);

    if (error) {
        std::cout
            << "SKIP: symlink escape test unavailable: "
            << error.message() << '\n';
        return;
    }

    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);
    dukeai::DocumentLoader document_loader;
    dukeai::DocumentMetadataLoader metadata_loader;

    dukeai::DocumentIngestion ingestion(
        document_loader,
        metadata_loader,
        approved_root);

    bool rejected = false;

    try {
        ingestion.ingest(
            linked_text.string(),
            metadata_path.string(),
            retriever);
    } catch (const std::exception&) {
        rejected = true;
    }

    check(rejected,
          "Symlink escape must be rejected.");

    check(
        retriever.passage_count() == 0,
        "Symlink rejection must not modify the index.");

    std::cout << "PASS: symlink escape rejection\n";
}

} // namespace

int main() {
    try {
        TemporaryDirectory directory;

        test_successful_ingestion(directory);
        test_malformed_metadata(directory);
        test_missing_text_file(directory);
        test_duplicate_ingestion(directory);
        test_outside_root_rejection(directory);
        test_symlink_escape_rejection(directory);

        std::cout << "PASS: document ingestion tests ("
                  << checks << " checks)\n";

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
