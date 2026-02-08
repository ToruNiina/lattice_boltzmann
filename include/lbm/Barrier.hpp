#ifndef LATTICE_BOLTZMANN_BARRIER_HPP
#define LATTICE_BOLTZMANN_BARRIER_HPP

#include "Direction.hpp"
#include "Vector.hpp"

#include <array>

namespace lbm
{

struct Barrier
{
    double  distribution(const Direction dir) const noexcept
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        return distribution_[static_cast<std::size_t>(dir)];
    }
    double& distribution(const Direction dir) noexcept
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        return distribution_[static_cast<std::size_t>(dir)];
    }

    // no fluid inside the barrier
    double density() const
    {
        return 0;
    }
    Vector velocity(const double) const
    {
        return Vector{0, 0};
    }

  private:

    std::array<double, 9> distribution_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_BOUNDARY_HPP
