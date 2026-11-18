#pragma once

#include "dukeai/energy_ranker.hpp"

#include <string>

namespace dukeai {

// Produces the three features consumed by EnergyRanker:
// 0: unique query-term overlap with document
// 1: unique query-term overlap with title
// 2: exact normalized language-tag match
Features extract_features(
    const std::string& query,
    const std::string& title,
    const std::string& document,
    const std::string& query_language,
    const std::string& document_language);

}  // namespace dukeai
