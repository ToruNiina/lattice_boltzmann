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
        for(const auto dir : all_dirs)
        {
            this->set_distribution(dir, 0);
        }
    }

    double  distribution(const Direction dir) const noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        return distribution_[static_cast<std::size_t>(dir)];
    }
    void set_distribution(const Direction dir, double d) noexcept override
    {
        assert(static_cast<std::size_t>(dir) < distribution_.size());
        distribution_[static_cast<std::size_t>(dir)] = d;
    }

    bool bounces() const noexcept override {return true;}
    std::pair<Direction, double> bounce_back(const Direction dir) noexcept override
    {
        const auto i = static_cast<std::size_t>(dir);
        assert(i < distribution_.size());

        const auto d = distribution_[i];
        distribution_[i] = 0;

        const auto back = ::lbm::bounce_back(dir);
        return {back, d};
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
