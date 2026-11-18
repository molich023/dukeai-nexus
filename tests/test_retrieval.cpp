#include "dukeai/document_metadata.hpp"
#include "dukeai/energy_ranker.hpp"
#include "dukeai/retriever.hpp"
#include "dukeai/document_loader.hpp"

#include <filesystem>
#include <fstream>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

static_assert(
    !std::is_constructible<
        dukeai::Retriever,
        dukeai::EnergyRanker&&>::value,
    "Retriever must reject temporary EnergyRanker objects");

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void test_document_ingestion() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "automata101",
        "Introduction to Automata",
        "en",
        "A finite automaton has states and transitions.\n\n"
        "A regular language is recognized by a finite automaton.");

    expect(
        retriever.passage_count() == 2,
        "Expected two passages after paragraph segmentation");
}

void test_search_and_source_traceability() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "automata101",
        "Introduction to Automata",
        "en",
        "A finite automaton has states and transitions.\n\n"
        "A regular language is recognized by a finite automaton.");

    const auto results =
        retriever.search("finite automaton", "en", 5);

    expect(!results.empty(), "Expected matching retrieval results");

    expect(
        results[0].passage.source_id == "automata101",
        "Source ID was not preserved");

    expect(
        results[0].passage.passage_id == "automata101:p1" ||
        results[0].passage.passage_id == "automata101:p2",
        "Passage ID was not preserved");

    expect(
        results[0].passage.position >= 1,
        "Passage position must be one-based");

    expect(
        results[0].features[0] > 0.0,
        "Retrieved passage should have lexical overlap");
}

void test_loaded_document_ingestion() {
    namespace fs = std::filesystem;

    const fs::path directory =
        fs::temp_directory_path() / "dukeai_retriever_loader_tests";

    fs::create_directories(directory);

    const fs::path file = directory / "automata.txt";

    const std::string content =
        "Finite automata contain states and transitions.\n\n"
        "Finite automata recognize regular languages.";

    {
        std::ofstream output(file, std::ios::binary);
        expect(static_cast<bool>(output), "Could not create integration fixture");

        output.write(
            content.data(),
            static_cast<std::streamsize>(content.size()));

        expect(static_cast<bool>(output), "Could not write integration fixture");
    }

    try {
        dukeai::DocumentLoader loader;
        dukeai::EnergyRanker ranker;
        dukeai::Retriever retriever(ranker);

        const auto document = loader.load_text_file(
            file.string(),
            "loaded-automata",
            "Automata from File",
            "en");

        retriever.add_document(document);

        expect(
            retriever.passage_count() == 2,
            "Loaded document should create two passages");

        const auto results =
            retriever.search("finite automata", "en");

        expect(
            !results.empty(),
            "Loaded document should be searchable");

        expect(
            results[0].passage.source_id == "loaded-automata",
            "Loaded source ID was not preserved");

        expect(
            results[0].passage.title == "Automata from File",
            "Loaded title was not preserved");

        expect(
            results[0].passage.language == "en",
            "Loaded language was not preserved");

        expect(
            results[0].passage.file_path == file.string(),
            "Loaded file path was not preserved");

        expect(
            results[0].passage.file_size_bytes == content.size(),
            "Loaded file size was not preserved");

        for (const auto& result : results) {
            expect(
                result.passage.file_path == file.string(),
                "A retrieved passage lost its file path");

            expect(
                result.passage.file_size_bytes == content.size(),
                "A retrieved passage lost its file size");
        }

        expect(
            results[0].passage.passage_id == "loaded-automata:p1" ||
            results[0].passage.passage_id == "loaded-automata:p2",
            "Loaded passage ID was not traceable");

        expect(
            results[0].passage.position >= 1,
            "Loaded passage position should be one-based");

        bool duplicate_rejected = false;

        try {
            retriever.add_document(document);
        } catch (const std::invalid_argument&) {
            duplicate_rejected = true;
        }

        expect(
            duplicate_rejected,
            "Duplicate loaded document should be rejected");

        expect(
            retriever.passage_count() == 2,
            "Duplicate loaded document changed passage count");

        expect(
            retriever.search("finite automata", "en").size() == 2,
            "Original loaded passages should remain searchable");
    } catch (...) {
        fs::remove_all(directory);
        throw;
    }

    fs::remove_all(directory);
}

void test_metadata_aware_ingestion() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    dukeai::LoadedDocument document;
    document.source_id = "metadata-source";
    document.title = "Automata Metadata Test";
    document.language = "en";
    document.content =
        "Finite automata contain states and transitions.\n\n"
        "Finite automata recognize regular languages.";
    document.file_path = "metadata-test.txt";
    document.file_size_bytes = document.content.size();

    const auto make_metadata =
        [](const dukeai::LoadedDocument& loaded) {
            dukeai::DocumentMetadata metadata;
            metadata.source_id = loaded.source_id;
            metadata.title = loaded.title;
            metadata.language = loaded.language;
            metadata.rights_status = "UNVERIFIED";
            metadata.redistribution_allowed = false;
            metadata.public_training_use = false;
            metadata.extraction_status = "COMPLETED";
            metadata.review_status = "PENDING";
            return metadata;
        };

    const auto valid_metadata = make_metadata(document);

    retriever.add_document(document, valid_metadata);

    expect(
        retriever.passage_count() == 2,
        "Metadata-aware ingestion should create two passages");

    const auto results = retriever.search("finite automata", "en");

    expect(
        results.size() == 2,
        "Metadata-aware document should be searchable");

    expect(
        results[0].passage.source_id == document.source_id,
        "Metadata-aware ingestion lost source ID");

    expect(
        results[0].passage.title == document.title,
        "Metadata-aware ingestion lost title");

    expect(
        results[0].passage.language == document.language,
        "Metadata-aware ingestion lost language");

    expect(
        results[0].passage.file_path == document.file_path,
        "Metadata-aware ingestion lost file path");

    expect(
        results[0].passage.file_size_bytes == document.file_size_bytes,
        "Metadata-aware ingestion lost file size");

    const auto expect_rejected_without_change =
        [&](const dukeai::LoadedDocument& candidate,
            const dukeai::DocumentMetadata& metadata,
            const std::string& description) {
            bool rejected = false;

            try {
                retriever.add_document(candidate, metadata);
            } catch (const std::invalid_argument&) {
                rejected = true;
            }

            expect(rejected, description + " should be rejected");

            expect(
                retriever.passage_count() == 2,
                description + " changed the existing passage count");
        };

    auto invalid_document = document;
    invalid_document.source_id = "invalid-rights-source";
    auto invalid_metadata = make_metadata(invalid_document);
    invalid_metadata.rights_status.clear();

    expect_rejected_without_change(
        invalid_document,
        invalid_metadata,
        "Invalid metadata");

    auto mismatched_source_document = document;
    mismatched_source_document.source_id = "source-match-test";
    auto mismatched_source_metadata =
        make_metadata(mismatched_source_document);
    mismatched_source_metadata.source_id = "different-source";

    expect_rejected_without_change(
        mismatched_source_document,
        mismatched_source_metadata,
        "Mismatched source ID");

    auto mismatched_title_metadata = make_metadata(document);
    mismatched_title_metadata.title = "Different title";

    expect_rejected_without_change(
        document,
        mismatched_title_metadata,
        "Mismatched title");

    auto mismatched_language_metadata = make_metadata(document);
    mismatched_language_metadata.language = "sw";

    expect_rejected_without_change(
        document,
        mismatched_language_metadata,
        "Mismatched language");

    expect_rejected_without_change(
        document,
        valid_metadata,
        "Duplicate metadata-aware document");
}

void test_direct_document_ingestion_still_works() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "direct-source",
        "Direct Ingestion",
        "en",
        "Finite automata recognize regular languages.");

    expect(
        retriever.passage_count() == 1,
        "Existing direct ingestion interface should remain functional");

    const auto results = retriever.search("automata", "en");

    expect(
        !results.empty(),
        "Directly ingested document should remain searchable");

    expect(
        results[0].passage.file_path.empty(),
        "Direct ingestion should not invent a file path");

    expect(
        results[0].passage.file_size_bytes == 0,
        "Direct ingestion should use zero for unknown file size");
}

void test_unrelated_query() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "automata101",
        "Introduction to Automata",
        "en",
        "A finite automaton has states and transitions.");

    const auto results =
        retriever.search("quantum chemistry", "en");

    expect(
        results.empty(),
        "Unrelated query should return no lexical candidates");
}

void test_result_limit() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "automata101",
        "Introduction to Automata",
        "en",
        "Finite automata contain states.\n\n"
        "Finite automata contain transitions.\n\n"
        "Finite automata recognize regular languages.");

    const auto results =
        retriever.search("finite automata", "en", 2);

    expect(results.size() == 2, "Result limit was not respected");
}

void test_empty_query() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "automata101",
        "Introduction to Automata",
        "en",
        "Finite automata contain states.");

    expect(
        retriever.search("", "en").empty(),
        "Empty query should return no results");
}

void test_invalid_source() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    bool rejected = false;

    try {
        retriever.add_document(
            "",
            "Automata",
            "en",
            "Finite automata contain states.");
    } catch (const std::invalid_argument&) {
        rejected = true;
    }

    expect(rejected, "Empty source ID should be rejected");
}

void test_duplicate_source_rejected() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "automata101",
        "Introduction to Automata",
        "en",
        "Finite automata contain states.");

    bool rejected = false;

    try {
        retriever.add_document(
            "automata101",
            "Another Automata Document",
            "en",
            "Finite automata contain transitions.");
    } catch (const std::invalid_argument&) {
        rejected = true;
    }

    expect(rejected, "Duplicate source ID should be rejected");

    expect(
        retriever.passage_count() == 1,
        "Duplicate ingestion changed the existing passage count");

    const auto results =
        retriever.search("states", "en");

    expect(
        results.size() == 1,
        "Original passage should remain available after rejection");

    expect(
        results[0].passage.passage_id == "automata101:p1",
        "Original passage ID changed after duplicate rejection");
}

void test_empty_document_source_is_registered() {
    dukeai::EnergyRanker ranker;
    dukeai::Retriever retriever(ranker);

    retriever.add_document(
        "empty-source",
        "Empty Document",
        "en",
        "\n   \n");

    expect(
        retriever.passage_count() == 0,
        "Empty document should not create passages");

    bool rejected = false;

    try {
        retriever.add_document(
            "empty-source",
            "Second Empty Document",
            "en",
            "This must not be accepted.");
    } catch (const std::invalid_argument&) {
        rejected = true;
    }

    expect(
        rejected,
        "Duplicate empty-document source ID should be rejected");

    expect(
        retriever.passage_count() == 0,
        "Rejected empty-document duplicate changed passage count");
}

}  // namespace

int main() {
    try {
        test_document_ingestion();
        test_loaded_document_ingestion();
        test_metadata_aware_ingestion();
        test_direct_document_ingestion_still_works();
        test_search_and_source_traceability();
        test_unrelated_query();
        test_result_limit();
        test_empty_query();
        test_invalid_source();
        test_duplicate_source_rejected();
        test_empty_document_source_is_registered();

        std::cout << "PASS: document ingestion\n";
        std::cout << "PASS: file-to-retriever integration\n";
        std::cout << "PASS: metadata-aware ingestion and validation\n";
        std::cout << "PASS: direct ingestion compatibility\n";
        std::cout << "PASS: source traceability\n";
        std::cout << "PASS: lexical retrieval\n";
        std::cout << "PASS: result limits\n";
        std::cout << "PASS: empty query handling\n";
        std::cout << "PASS: input validation\n";
        std::cout << "PASS: duplicate source rejection\n";
        std::cout << "PASS: empty-document source registration\n";
        std::cout << "PASS: temporary ranker protection\n";
        std::cout << "PASS: all retrieval tests\n";

        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
