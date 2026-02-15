#ifndef LATTICE_BOLTZMANN_BGK_HPP
#define LATTICE_BOLTZMANN_BGK_HPP

#include "Direction.hpp"
#include "GridBase.hpp"

#include <cassert>

namespace lbm
{

struct BGK
{
    explicit BGK(const double nu)
        : viscosity_(nu), omega_(1.0 / (3.0 * nu + 0.5))
    {}

    void collide(GridBase& g, double rho, Vector u) const
    {
        for(const auto& dir : all_dirs)
        {
            auto d = g.distribution(dir);
            d *= (1 - omega_);
            d += omega_ * this->equilibrium(dir, rho, u);
            g.set_distribution(dir, d);
        }
        return ;
    }

    double equilibrium(const Direction dir, const double rho, const Vector u) const
    {
        const auto u2 = length_sq(u);

        using enum Direction;
        switch(dir)
        {
            case Self     : { return (4/ 9.0) * rho * (1 - 1.5 * u2); }

            case Right    : { return (1/ 9.0) * rho * (1 + 3*u.x + 4.5*u.x*u.x - 1.5*u2); }
            case Up       : { return (1/ 9.0) * rho * (1 + 3*u.y + 4.5*u.y*u.y - 1.5*u2); }
            case Left     : { return (1/ 9.0) * rho * (1 - 3*u.x + 4.5*u.x*u.x - 1.5*u2); }
            case Down     : { return (1/ 9.0) * rho * (1 - 3*u.y + 4.5*u.y*u.y - 1.5*u2); }

            case RightUp  : { return (1/36.0) * rho * (1 + 3*( u.x+u.y) + 4.5*( u.x+u.y)*( u.x+u.y) - 1.5 * u2); }
            case LeftUp   : { return (1/36.0) * rho * (1 + 3*(-u.x+u.y) + 4.5*(-u.x+u.y)*(-u.x+u.y) - 1.5 * u2); }
            case LeftDown : { return (1/36.0) * rho * (1 + 3*(-u.x-u.y) + 4.5*(-u.x-u.y)*(-u.x-u.y) - 1.5 * u2); }
            case RightDown: { return (1/36.0) * rho * (1 + 3*( u.x-u.y) + 4.5*( u.x-u.y)*( u.x-u.y) - 1.5 * u2); }
            default: break;
        }
        assert(false);
        return 0;
    }

  private:

    double viscosity_;
    double omega_; // 1/tau
};

} // lbm
#endif // LATTICE_BOLTZMANN_BGK_HPP
