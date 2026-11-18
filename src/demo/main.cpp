#include "dukeai/document_ingestion.hpp"
#include "dukeai/energy_ranker.hpp"
#include "dukeai/retriever.hpp"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct DemoQuery {
    std::string text;
    std::string language;
};

int main(int argc, char* argv[]) {
    try {
        const std::filesystem::path data_dir =
            argc > 1 ? argv[1] : "data/demo";

        dukeai::EnergyRanker ranker;
        dukeai::Retriever retriever(ranker);

        dukeai::DocumentLoader document_loader;
        dukeai::DocumentMetadataLoader metadata_loader;

        dukeai::DocumentIngestion ingestion(
            document_loader,
            metadata_loader
        );

        const auto english_text = data_dir / "english_water.txt";
        const auto english_metadata = data_dir / "english_water.json";
        const auto swahili_text = data_dir / "kiswahili_water.txt";
        const auto swahili_metadata = data_dir / "kiswahili_water.json";

        ingestion.ingest(
            english_text.string(),
            english_metadata.string(),
            retriever
        );

        ingestion.ingest(
            swahili_text.string(),
            swahili_metadata.string(),
            retriever
        );

        std::cout << "DukeAI Nexus — Retrieval Demonstration\n";
        std::cout << "======================================\n";
        std::cout << "Indexed passages: "
                  << retriever.passage_count() << '\n';
        std::cout << "Retrieval method: lexical candidate selection\n";
        std::cout << "Ranker status: untrained; deterministic fallback\n";
        std::cout << "Generation: not implemented\n\n";

        const std::vector<DemoQuery> queries = {
            {"water conservation", "English"},
            {"uhifadhi maji", "Kiswahili"}
        };

        for (const auto& query : queries) {
            std::cout << "Query: " << query.text << '\n';
            std::cout << "Query language: " << query.language << '\n';

            const auto results = retriever.search(
                query.text,
                query.language,
                3
            );

            if (results.empty()) {
                std::cout << "No matching passages found.\n\n";
                continue;
            }

            for (const auto& result : results) {
                std::cout << "\nSource ID: "
                          << result.passage.source_id << '\n';
                std::cout << "Title: "
                          << result.passage.title << '\n';
                std::cout << "Document language: "
                          << result.passage.language << '\n';
                std::cout << "Passage ID: "
                          << result.passage.passage_id << '\n';
                std::cout << "Energy: "
                          << std::fixed << std::setprecision(3)
                          << result.energy << '\n';
                std::cout << "Passage: "
                          << result.passage.text << '\n';
            }

            std::cout << "\n--------------------------------------\n";
        }

        std::cout << "Notice: retrieved passages are source text, "
                     "not generated answers.\n";

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Demo failed: " << error.what() << '\n';
        return 1;
    }
}
