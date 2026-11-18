#include "dukeai/document_metadata.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

int checks = 0;

void check(bool condition, const std::string& message) {
    ++checks;
    if (!condition) {
        throw std::runtime_error("FAIL: " + message);
    }
}

dukeai::DocumentMetadata valid_metadata() {
    dukeai::DocumentMetadata metadata;

    metadata.source_id = "tcs_mishra_chandrasekaran_2008";
    metadata.title =
        "Theory of Computer Science: Automata, Languages and Computation";
    metadata.authors = {"K. L. P. Mishra", "N. Chandrasekaran"};
    metadata.edition = "Third Edition";
    metadata.publication_year = 2008;
    metadata.publisher = "Prentice-Hall of India";
    metadata.document_format = "PDF";
    metadata.page_count = 434;
    metadata.language = "English";
    metadata.subjects = {
        "Computer Science",
        "Automata Theory",
        "Formal Languages",
        "Computation"
    };
    metadata.rights_status = "UNVERIFIED";
    metadata.redistribution_allowed = false;
    metadata.public_training_use = false;
    metadata.local_research_use =
        "Subject to applicable copyright and licence restrictions";
    metadata.extraction_status = "COMPLETED";
    metadata.extracted_words = 149116;
    metadata.review_status = "PENDING";

    return metadata;
}

void test_valid_metadata() {
    const auto metadata = valid_metadata();

    check(
        dukeai::validate_document_metadata(metadata).empty(),
        "valid metadata passes validation");

    std::cout << "PASS: valid metadata\n";
}

void test_required_fields() {
    auto metadata = valid_metadata();

    metadata.source_id = "  ";
    metadata.title.clear();
    metadata.language = "\t";
    metadata.rights_status.clear();
    metadata.review_status = " ";

    const auto errors =
        dukeai::validate_document_metadata(metadata);

    check(errors.size() == 5, "all missing required fields detected");

    std::cout << "PASS: required-field validation\n";
}

void test_optional_numeric_fields() {
    auto metadata = valid_metadata();

    metadata.publication_year.reset();
    metadata.page_count.reset();
    metadata.extracted_words.reset();

    check(
        dukeai::validate_document_metadata(metadata).empty(),
        "unspecified optional numeric fields are accepted");

    std::cout << "PASS: optional metadata fields\n";
}

void test_invalid_publication_year() {
    auto metadata = valid_metadata();
    metadata.publication_year = 0;

    const auto errors =
        dukeai::validate_document_metadata(metadata);

    check(errors.size() == 1, "invalid publication year detected");

    metadata.publication_year = 10000;

    check(
        dukeai::validate_document_metadata(metadata).size() == 1,
        "publication year above maximum rejected");

    std::cout << "PASS: publication-year validation\n";
}

void test_invalid_page_count() {
    auto metadata = valid_metadata();
    metadata.page_count = 0;

    check(
        !dukeai::validate_document_metadata(metadata).empty(),
        "zero page count rejected");

    std::cout << "PASS: page-count validation\n";
}

void test_unverified_rights() {
    auto metadata = valid_metadata();

    metadata.redistribution_allowed = true;

    check(
        !dukeai::validate_document_metadata(metadata).empty(),
        "unverified redistribution permission rejected");

    metadata.redistribution_allowed = false;
    metadata.public_training_use = true;

    check(
        !dukeai::validate_document_metadata(metadata).empty(),
        "unverified public training permission rejected");

    std::cout << "PASS: rights safeguards\n";
}

void test_extraction_status() {
    auto metadata = valid_metadata();
    metadata.extraction_status = " ";

    check(
        !dukeai::validate_document_metadata(metadata).empty(),
        "blank extraction status rejected");

    std::cout << "PASS: extraction-status validation\n";
}

void test_matching_source_ids() {
    const auto metadata = valid_metadata();

    check(
        dukeai::validate_document_match(
            metadata.source_id, metadata).empty(),
        "matching source IDs accepted");

    std::cout << "PASS: matching source IDs\n";
}

void test_mismatched_source_ids() {
    const auto metadata = valid_metadata();

    check(
        !dukeai::validate_document_match(
            "different_source", metadata).empty(),
        "mismatched source IDs rejected");

    check(
        !dukeai::validate_document_match(" ", metadata).empty(),
        "blank loaded source ID rejected");

    std::cout << "PASS: mismatched source IDs\n";
}

} // namespace

int main() {
    try {
        test_valid_metadata();
        test_required_fields();
        test_optional_numeric_fields();
        test_invalid_publication_year();
        test_invalid_page_count();
        test_unverified_rights();
        test_extraction_status();
        test_matching_source_ids();
        test_mismatched_source_ids();

        std::cout << "PASS: all metadata tests ("
                  << checks << " checks)\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
