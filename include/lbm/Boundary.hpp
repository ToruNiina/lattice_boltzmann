#ifndef LATTICE_BOLTZMANN_BOUNDARY_HPP
#define LATTICE_BOLTZMANN_BOUNDARY_HPP

#include "Direction.hpp"
#include "Vector.hpp"

#include <array>

namespace lbm
{

struct ConstantFlow final : public GridBase
{
    ~ConstantFlow() override = default;

    void initialize(const BGK& bgk, const double rho, const Vector u) override
    {
        this->density_  = rho;
        this->velocity_ = u;

        // equilibrium distribution

        for(const auto& dir : all_dirs)
        {
            this->true_distribution_.at(static_cast<std::size_t>(dir)) =
                bgk.equilibrium(dir, rho, u);
        }
        distribution_ = true_distribution_;
    }

    double  distribution(const Direction dir) const noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        distribution_ = true_distribution_;
        return distribution_[static_cast<std::size_t>(dir)];
    }
    double& distribution(const Direction dir) noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        distribution_ = true_distribution_;
        return distribution_[static_cast<std::size_t>(dir)];
    }

    // no fluid inside the barrier
    double density() const override
    {
        return density_;
    }
    Vector velocity() const override
    {
        return velocity(density());
    }
    Vector velocity(const double) const override
    {
        return velocity_;
    }

  private:

    double density_;
    Vector velocity_;

    std::array<double, 9> true_distribution_;
    mutable std::array<double, 9> distribution_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_BOUNDARY_HPP
