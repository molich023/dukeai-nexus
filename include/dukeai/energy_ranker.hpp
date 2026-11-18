#pragma once

#include <array>
#include <cstddef>

namespace dukeai {

using Features = std::array<double, 3>;

class EnergyRanker {
public:
    explicit EnergyRanker(double learning_rate = 0.05,
                          double margin = 0.2);

    double energy(const Features& features) const;
    double train_pair(const Features& positive,
                      const Features& negative);

    bool ranks_positive_lower(const Features& positive,
                              const Features& negative) const;

    const Features& weights() const;

private:
    Features weights_{0.0, 0.0, 0.0};
    double learning_rate_;
    double margin_;
};

}  // namespace dukeai
