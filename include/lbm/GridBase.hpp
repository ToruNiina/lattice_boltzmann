#ifndef LATTICE_BOLTZMANN_GRID_BASE_HPP
#define LATTICE_BOLTZMANN_GRID_BASE_HPP

#include "Direction.hpp"

#include <array>

namespace lbm
{

struct GridBase
{
    virtual ~GridBase() = default;

    void initialize(const BGK&, const double, const double Vector) = 0;

    virtual double  distribution(const Direction dir) const noexcept = 0;
    virtual double& distribution(const Direction dir)       noexcept = 0;

    double density() const = 0;
    Vector velocity(const double rho) const = 0;
    Vector velocity() const = 0;
};

} // lbm
#endif // LATTICE_BOLTZMANN_GRID_HPP
