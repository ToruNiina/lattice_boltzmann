#ifndef LATTICE_BOLTZMANN_GRID_BASE_HPP
#define LATTICE_BOLTZMANN_GRID_BASE_HPP

#include "Direction.hpp"
#include "Vector.hpp"

#include <array>

namespace lbm
{

struct BGK;

struct GridBase
{
    virtual ~GridBase() = default;

    virtual void initialize(const BGK&, const double, const Vector) = 0;

    virtual double  distribution(const Direction dir) const noexcept = 0;
    virtual double& distribution(const Direction dir)       noexcept = 0;

    virtual double density() const = 0;
    virtual Vector velocity(const double rho) const = 0;
    virtual Vector velocity() const = 0;
};

} // lbm
#endif // LATTICE_BOLTZMANN_GRID_HPP
