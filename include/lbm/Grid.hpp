#ifndef LATTICE_BOLTZMANN_GRID_HPP
#define LATTICE_BOLTZMANN_GRID_HPP

#include "Barrier.hpp"
#include "Cell.hpp"

#include <array>

namespace lbm
{

struct Grid
{
    explicit Grid(Cell    c): grid_(c) {}
    explicit Grid(Barrier b): grid_(b) {}
    explicit Grid& operator=(Cell    c) {grid_ = c; return *this;}
    explicit Grid& operator=(Barrier b) {grid_ = b; return *this;}

    void initialize(const BGK& model, const double rho, const double Vector u) override
    {
        return std::visit([&model, rho, u](auto& g) {return g.initialize(model, rho, u);});
    }

    double  distribution(const Direction dir) const noexcept
    {
        return std::visit([dir](const auto& g) {return g.distribution(dir);});
    }
    double& distribution(const Direction dir) noexcept
    {
        return std::visit([dir](auto& g) {return g.distribution(dir);});
    }

    double density() const
    {
        return std::visit([](const auto& g) {return g.density();});
    }
    Vector velocity() const override
    {
        return velocity(density());
    }
    Vector velocity(const double rho) const
    {
        return std::visit([rho](const auto& g) {return g.velocity(rho);});
    }

  private:

    std::variant<Cell, Barrier> grid_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_GRID_HPP
