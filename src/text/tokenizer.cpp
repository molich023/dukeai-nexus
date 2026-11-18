#include "dukeai/tokenizer.hpp"

#include <cctype>
#include <string>
#include <vector>

namespace dukeai {
namespace {

bool is_token_byte(unsigned char value) {
    // Preserve non-ASCII bytes so UTF-8 text is not discarded.
    if (value >= 0x80) {
        return true;
    }

    return std::isalnum(value) != 0;
}

char lowercase_ascii(unsigned char value) {
    if (value >= 'A' && value <= 'Z') {
        return static_cast<char>(value - 'A' + 'a');
    }

    return static_cast<char>(value);
}

}  // namespace

std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string current;

    for (unsigned char value : text) {
        if (is_token_byte(value)) {
            current.push_back(lowercase_ascii(value));
        } else if (!current.empty()) {
            tokens.push_back(current);
            current.clear();
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

}  // namespace dukeai
