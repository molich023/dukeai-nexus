#include "dukeai/energy_ranker.hpp"

#include <stdexcept>

namespace dukeai {

EnergyRanker::EnergyRanker(double learning_rate, double margin)
    : learning_rate_(learning_rate), margin_(margin) {
    if (learning_rate <= 0.0 || margin <= 0.0) {
        throw std::invalid_argument(
            "Learning rate and margin must be positive.");
    }
}

double EnergyRanker::energy(const Features& features) const {
    double score = 0.0;

    for (std::size_t i = 0; i < weights_.size(); ++i) {
        score += weights_[i] * features[i];
    }

    // Lower energy means a more preferred candidate.
    return -score;
}

double EnergyRanker::train_pair(const Features& positive,
                                const Features& negative) {
    const double positive_energy = energy(positive);
    const double negative_energy = energy(negative);

    const double loss =
        margin_ + positive_energy - negative_energy;

    if (loss > 0.0) {
        // SGD update for the pairwise margin-ranking objective.
        for (std::size_t i = 0; i < weights_.size(); ++i) {
            weights_[i] += learning_rate_ *
                           (positive[i] - negative[i]);
        }
    }

    return loss > 0.0 ? loss : 0.0;
}

bool EnergyRanker::ranks_positive_lower(
    const Features& positive,
    const Features& negative) const {
    return energy(positive) < energy(negative);
}

const Features& EnergyRanker::weights() const {
    return weights_;
}

}  // namespace dukeai
