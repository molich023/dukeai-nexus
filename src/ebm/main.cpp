#include "dukeai/energy_ranker.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Pair {
    dukeai::Features positive;
    dukeai::Features negative;
};

int main(int argc, char* argv[]) {
    const std::string path =
        argc > 1 ? argv[1] : "tests/data/ranking_pairs.tsv";

    std::ifstream input(path);
    if (!input) {
        std::cerr << "Cannot open training data: " << path << '\n';
        return 1;
    }

    std::vector<Pair> pairs;
    std::string line;

    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream row(line);
        Pair p{};

        if (!(row >> p.positive[0] >> p.positive[1]
                  >> p.positive[2] >> p.negative[0]
                  >> p.negative[1] >> p.negative[2])) {
            std::cerr << "Invalid data row: " << line << '\n';
            return 2;
        }

        std::string extra;
        if (row >> extra) {
            std::cerr << "Unexpected extra field: " << line << '\n';
            return 2;
        }

        pairs.push_back(p);
    }

    if (pairs.empty()) {
        std::cerr << "No training pairs found.\n";
        return 3;
    }

    dukeai::EnergyRanker model(0.05, 0.2);
    constexpr int epochs = 100;

    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (const auto& pair : pairs) {
            model.train_pair(pair.positive, pair.negative);
        }
    }

    std::size_t correct = 0;
    for (const auto& pair : pairs) {
        if (model.ranks_positive_lower(
                pair.positive, pair.negative)) {
            ++correct;
        }
    }

    const auto& w = model.weights();

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "model=linear_energy_ranker\n";
    std::cout << "training_pairs=" << pairs.size() << '\n';
    std::cout << "epochs=" << epochs << '\n';
    std::cout << "weight_overlap=" << w[0] << '\n';
    std::cout << "weight_title=" << w[1] << '\n';
    std::cout << "weight_language=" << w[2] << '\n';
    std::cout << "training_pair_accuracy="
              << static_cast<double>(correct) / pairs.size()
              << '\n';
    std::cout << "warning=training accuracy is not held-out accuracy\n";

    return 0;
}
