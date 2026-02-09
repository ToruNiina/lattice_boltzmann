#ifndef LATTICE_BOLTZMANN_BARRIER_HPP
#define LATTICE_BOLTZMANN_BARRIER_HPP

#include "Direction.hpp"
#include "Vector.hpp"

#include <array>

namespace lbm
{

struct Barrier final : public GridBase
{
    ~Barrier() override = default;

    void initialize(const BGK&, const double, const Vector) override
    {
        for(const auto& dir : all_dirs)
        {
            this->distribution(dir) = 0;
        }
    }

    double  distribution(const Direction dir) const noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        return distribution_[static_cast<std::size_t>(dir)];
    }
    double& distribution(const Direction dir) noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        return distribution_[static_cast<std::size_t>(dir)];
    }

    // no fluid inside the barrier
    double density() const override
    {
        return 0;
    }
    Vector velocity() const override
    {
        return velocity(density());
    }
    Vector velocity(const double) const override
    {
        return Vector{0, 0};
    }

  private:

    std::array<double, 9> distribution_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_BOUNDARY_HPP
