#ifndef LATTICE_BOLTZMANN_GRID_HPP
#define LATTICE_BOLTZMANN_GRID_HPP

#include "Barrier.hpp"
#include "Cell.hpp"
#include "Boundary.hpp"

#include <variant>
#include <array>

namespace lbm
{


struct Grid final : public GridBase
{
    Grid(): grid_(Cell{}) {}

    explicit Grid(Cell    c): grid_(c) {}
    explicit Grid(Barrier b): grid_(b) {}
    explicit Grid(ConstantFlow c): grid_(c) {}
    Grid& operator=(Cell    c) {grid_ = c; return *this;}
    Grid& operator=(Barrier b) {grid_ = b; return *this;}
    Grid& operator=(ConstantFlow c) {grid_ = c; return *this;}

    void initialize(const BGK& model, const double rho, const Vector u) override
    {
        return std::visit([&model, rho, u](auto& g) {return g.initialize(model, rho, u);}, grid_);
    }

    double  distribution(const Direction dir) const noexcept override
    {
        return std::visit([dir](const auto& g) {return g.distribution(dir);}, grid_);
    }
    double& distribution(const Direction dir) noexcept override
    {
        return std::visit(DistributionGetter{dir}, grid_);
    }

    double density() const override
    {
        return std::visit([](const auto& g) {return g.density();}, grid_);
    }
    Vector velocity() const override
    {
        return velocity(density());
    }
    Vector velocity(const double rho) const override
    {
        return std::visit([rho](const auto& g) {return g.velocity(rho);}, grid_);
    }

    bool is_cell()    const noexcept {return grid_.index() == 0;}
    bool is_barrier() const noexcept {return grid_.index() == 1;}

  private:

    struct DistributionGetter
    {
        template<typename T>
        double& operator()(T& c) const {return c.distribution(d);}

        Direction d;
    };

  private:

    std::variant<Cell, Barrier, ConstantFlow> grid_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_GRID_HPP
