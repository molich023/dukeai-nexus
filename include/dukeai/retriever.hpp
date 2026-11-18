#pragma once

#include "dukeai/energy_ranker.hpp"
#include "dukeai/document_loader.hpp"
#include "dukeai/document_metadata.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace dukeai {

struct Passage {
    std::string passage_id;
    std::string source_id;
    std::string title;
    std::string language;
    std::string file_path;
    std::size_t file_size_bytes = 0;
    std::size_t position;
    std::string text;
};

struct RetrievalResult {
    Passage passage;
    Features features;
    double energy;
};

class Retriever {
public:
    // Non-owning reference: ranker must outlive this Retriever.
    explicit Retriever(const EnergyRanker& ranker);

    // Prevent binding the reference to a temporary EnergyRanker.
    Retriever(EnergyRanker&& ranker) = delete;

    void add_document(
        const std::string& source_id,
        const std::string& title,
        const std::string& language,
        const std::string& content);

    // Ingest a document that has passed DocumentLoader validation.
    void add_document(const LoadedDocument& document);

    // Ingest only when its metadata is valid and matches the document.
    void add_document(
        const LoadedDocument& document,
        const DocumentMetadata& metadata);

    std::vector<RetrievalResult> search(
        const std::string& query,
        const std::string& query_language,
        std::size_t limit = 5) const;

    std::size_t passage_count() const;

private:
    void add_document_impl(
        const std::string& source_id,
        const std::string& title,
        const std::string& language,
        const std::string& content,
        const std::string& file_path,
        std::size_t file_size_bytes);

    const EnergyRanker& ranker_;
    std::vector<std::string> source_ids_;
    std::vector<Passage> passages_;
};

}  // namespace dukeai
