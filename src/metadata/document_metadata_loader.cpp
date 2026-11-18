#include "dukeai/document_metadata_loader.hpp"

#include <json/json.h>

#include <cstdint>
#include <fstream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace dukeai {
namespace {

constexpr std::size_t MAX_METADATA_FILE_SIZE = 1024 * 1024;

std::string required_string(
    const Json::Value& root,
    const std::string& key) {

    if (!root.isMember(key) || !root[key].isString()) {
        throw std::runtime_error(
            "Missing or invalid string field: " + key);
    }

    return root[key].asString();
}

std::string optional_string(
    const Json::Value& root,
    const std::string& key) {

    if (!root.isMember(key)) {
        return {};
    }

    if (!root[key].isString()) {
        throw std::runtime_error(
            "Invalid string field: " + key);
    }

    return root[key].asString();
}

bool optional_bool(
    const Json::Value& root,
    const std::string& key) {

    if (!root.isMember(key)) {
        return false;
    }

    if (!root[key].isBool()) {
        throw std::runtime_error(
            "Invalid boolean field: " + key);
    }

    return root[key].asBool();
}

std::optional<int> optional_int(
    const Json::Value& root,
    const std::string& key) {

    if (!root.isMember(key)) {
        return std::nullopt;
    }

    const Json::Value& value = root[key];

    if (!value.isInt()) {
        throw std::runtime_error(
            "Invalid integer field: " + key);
    }

    return value.asInt();
}

std::optional<std::size_t> optional_size(
    const Json::Value& root,
    const std::string& key) {

    if (!root.isMember(key)) {
        return std::nullopt;
    }

    const Json::Value& value = root[key];

    if (!value.isUInt64()) {
        throw std::runtime_error(
            "Invalid non-negative integer field: " + key);
    }

    const Json::UInt64 number = value.asUInt64();

    if (number >
        static_cast<Json::UInt64>(
            std::numeric_limits<std::size_t>::max())) {
        throw std::runtime_error(
            "Integer field exceeds platform size: " + key);
    }

    return static_cast<std::size_t>(number);
}

std::vector<std::string> optional_string_array(
    const Json::Value& root,
    const std::string& key) {

    std::vector<std::string> result;

    if (!root.isMember(key)) {
        return result;
    }

    const Json::Value& value = root[key];

    if (!value.isArray()) {
        throw std::runtime_error(
            "Invalid array field: " + key);
    }

    for (Json::ArrayIndex i = 0; i < value.size(); ++i) {
        if (!value[i].isString()) {
            throw std::runtime_error(
                "Array contains a non-string value: " + key);
        }

        result.push_back(value[i].asString());
    }

    return result;
}

void reject_unknown_fields(const Json::Value& root) {
    static const std::vector<std::string> allowed = {
        "source_id",
        "title",
        "authors",
        "edition",
        "publication_year",
        "publisher",
        "document_format",
        "page_count",
        "language",
        "subject",
        "subjects",
        "source_url",
        "rights_status",
        "redistribution_allowed",
        "public_training_use",
        "local_research_use",
        "extraction_status",
        "extracted_words",
        "review_status"
    };

    for (const auto& key : root.getMemberNames()) {
        bool known = false;

        for (const auto& field : allowed) {
            if (key == field) {
                known = true;
                break;
            }
        }

        if (!known) {
            throw std::runtime_error(
                "Unknown metadata field: " + key);
        }
    }
}

} // namespace

DocumentMetadata DocumentMetadataLoader::load_json_file(
    const std::string& file_path) const {

    if (file_path.empty()) {
        throw std::invalid_argument(
            "Metadata file path must not be empty");
    }

    std::ifstream file(file_path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Unable to open metadata file: " + file_path);
    }

    std::ostringstream buffer;
    char chunk[4096];
    std::size_t total = 0;

    while (file.read(chunk, sizeof(chunk)) || file.gcount() > 0) {
        const auto count =
            static_cast<std::size_t>(file.gcount());

        if (count > MAX_METADATA_FILE_SIZE - total) {
            throw std::runtime_error(
                "Metadata file exceeds 1 MiB limit");
        }

        buffer.write(chunk, static_cast<std::streamsize>(count));
        total += count;
    }

    if (file.bad()) {
        throw std::runtime_error(
            "Failed while reading metadata file: " + file_path);
    }

    Json::CharReaderBuilder builder;

    builder["failIfExtra"] = true;
    builder["allowComments"] = false;
    builder["collectComments"] = false;
    builder["allowTrailingCommas"] = false;
    builder["strictRoot"] = true;
    builder["allowDroppedNullPlaceholders"] = false;
    builder["allowNumericKeys"] = false;
    builder["rejectDupKeys"] = true;
    builder["allowSpecialFloats"] = false;

    Json::Value root;
    std::string errors;
    std::istringstream input(buffer.str());

    if (!Json::parseFromStream(builder, input, &root, &errors)) {
        throw std::runtime_error(
            "Invalid metadata JSON: " + errors);
    }

    if (!root.isObject()) {
        throw std::runtime_error(
            "Metadata JSON root must be an object");
    }

    reject_unknown_fields(root);

    if (root.isMember("subject") && root.isMember("subjects")) {
        throw std::runtime_error(
            "Use either subject or subjects, not both");
    }

    DocumentMetadata metadata;

    metadata.source_id = required_string(root, "source_id");
    metadata.title = required_string(root, "title");
    metadata.language = required_string(root, "language");
    metadata.rights_status = required_string(root, "rights_status");
    metadata.extraction_status =
        required_string(root, "extraction_status");
    metadata.review_status = required_string(root, "review_status");

    metadata.authors = optional_string_array(root, "authors");

    metadata.subjects = root.isMember("subjects")
        ? optional_string_array(root, "subjects")
        : optional_string_array(root, "subject");

    metadata.edition = optional_string(root, "edition");
    metadata.publisher = optional_string(root, "publisher");
    metadata.document_format =
        optional_string(root, "document_format");
    metadata.source_url = optional_string(root, "source_url");
    metadata.local_research_use =
        optional_string(root, "local_research_use");

    metadata.publication_year =
        optional_int(root, "publication_year");
    metadata.page_count = optional_size(root, "page_count");
    metadata.extracted_words = optional_size(root, "extracted_words");

    metadata.redistribution_allowed =
        optional_bool(root, "redistribution_allowed");
    metadata.public_training_use =
        optional_bool(root, "public_training_use");

    const auto validation_errors =
        validate_document_metadata(metadata);

    if (!validation_errors.empty()) {
        throw std::runtime_error(
            "Metadata validation failed: " +
            validation_errors.front());
    }

    return metadata;
}

} // namespace dukeai
