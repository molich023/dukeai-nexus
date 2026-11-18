#include "dukeai/feature_extractor.hpp"
#include "dukeai/tokenizer.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void expect(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void expect_near(
    double actual,
    double expected,
    const std::string& message) {

    constexpr double tolerance = 1e-9;

    expect(std::abs(actual - expected) < tolerance, message);
}

void test_tokenizer() {
    const auto tokens =
        dukeai::tokenize("Automata, THEORY! Languages-AND computation.");

    const std::vector<std::string> expected{
        "automata", "theory", "languages", "and", "computation"
    };

    expect(tokens == expected, "Tokenizer normalization failed");

    const auto repeated =
        dukeai::tokenize("DukeAI dukeai Nexus");

    expect(
        repeated == std::vector<std::string>{
            "dukeai", "dukeai", "nexus"
        },
        "Tokenizer should preserve repeated terms");
}

void test_feature_extraction() {
    const auto features = dukeai::extract_features(
        "Automata automata theory",
        "Theory of Computation",
        "Automata and formal languages",
        "EN",
        "en");

    expect_near(
        features[0], 0.5,
        "Document overlap should be 0.5");

    expect_near(
        features[1], 0.5,
        "Title overlap should be 0.5");

    expect_near(
        features[2], 1.0,
        "Matching language tags should produce 1.0");
}

void test_language_mismatch() {
    const auto features = dukeai::extract_features(
        "habari dunia",
        "Habari za Dunia",
        "Habari dunia ya leo",
        "sw",
        "en");

    expect_near(
        features[2], 0.0,
        "Different language tags should produce 0.0");
}

void test_empty_query() {
    const auto features = dukeai::extract_features(
        "",
        "Automata Theory",
        "Automata and formal languages",
        "en",
        "en");

    expect_near(features[0], 0.0, "Empty query document overlap");
    expect_near(features[1], 0.0, "Empty query title overlap");
    expect_near(features[2], 1.0, "Language match is independent");
}

void test_empty_language() {
    const auto features = dukeai::extract_features(
        "automata",
        "Automata",
        "Automata theory",
        "",
        "en");

    expect_near(
        features[2], 0.0,
        "Empty language tags must not match");
}

}  // namespace

int main() {
    try {
        test_tokenizer();
        test_feature_extraction();
        test_language_mismatch();
        test_empty_query();
        test_empty_language();

        std::cout << "PASS: tokenizer tests\n";
        std::cout << "PASS: feature extraction tests\n";
        std::cout << "PASS: language matching tests\n";
        std::cout << "PASS: empty input tests\n";
        std::cout << "PASS: all text tests\n";

        return EXIT_SUCCESS;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
