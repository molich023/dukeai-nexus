#pragma once

#include <string>
#include <vector>

namespace dukeai {

// Conservative whitespace/punctuation tokenizer.
// ASCII letters are lowercased; non-ASCII UTF-8 bytes are preserved.
std::vector<std::string> tokenize(const std::string& text);

}  // namespace dukeai
