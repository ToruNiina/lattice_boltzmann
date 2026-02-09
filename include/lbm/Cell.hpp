#ifndef LATTICE_BOLTZMANN_CELL_HPP
#define LATTICE_BOLTZMANN_CELL_HPP

#include "Direction.hpp"
#include "Vector.hpp"

#include <array>

namespace lbm
{

struct Cell final : public GridBase
{
    ~Cell() override = default;

    void initialize(const BGK& model, const double rho, const Vector u) override
    {
        for(const auto& dir : all_dirs)
        {
            this->distribution(dir) = model.equilibrium(dir, rho, u);
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

    double density() const override
    {
        double d = 0;
        for(const auto& distr : distribution_)
        {
            d += distr;
        }
        return d;
    }
    Vector velocity() const override
    {
        return velocity(density());
    }
    Vector velocity(const double rho) const override
    {
        const auto rho_inv = 1.0 / rho;

        const auto x_elem = this->distribution(Direction::Right)
                          + this->distribution(Direction::RightUp)
                          + this->distribution(Direction::RightDown)
                          - this->distribution(Direction::Left)
                          - this->distribution(Direction::LeftUp)
                          - this->distribution(Direction::LeftDown) ;

        const auto y_elem = this->distribution(Direction::RightUp)
                          + this->distribution(Direction::Up)
                          + this->distribution(Direction::LeftUp)
                          - this->distribution(Direction::RightDown)
                          - this->distribution(Direction::Down)
                          - this->distribution(Direction::LeftDown) ;

        return Vector{
            rho_inv * x_elem,
            rho_inv * y_elem
        };
    }

  private:

    std::array<double, 9> distribution_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_CELL_HPP
