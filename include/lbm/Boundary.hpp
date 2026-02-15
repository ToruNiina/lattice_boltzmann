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
            this->distribution_.at(static_cast<std::size_t>(dir)) =
                bgk.equilibrium(dir, rho, u);
        }
        this->eq_distribution_ = this->distribution_; // keep it
    }

    double  distribution(const Direction dir) const noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        return distribution_[static_cast<std::size_t>(dir)];
    }
    void set_distribution(const Direction dir, double d) noexcept override
    {
        const auto back = ::lbm::bounce_back(dir);
        const auto dir_idx  = static_cast<std::size_t>(dir);
        const auto back_idx = static_cast<std::size_t>(back);

        assert(dir_idx  < distribution_.size());
        assert(back_idx < distribution_.size());

        // bounce non-equilibrium part of the distribution

        distribution_[dir_idx ] = d;
        distribution_[back_idx] = eq_distribution_[back_idx] + (d - eq_distribution_[dir_idx]);

        return ;
    }

    bool bounces() const noexcept override {return false;}

    std::pair<Direction, double> bounce_back(const Direction dir) noexcept override
    {
        return {Direction::None, 0};
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

    std::array<double, 9> eq_distribution_;
    std::array<double, 9> distribution_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_BOUNDARY_HPP
