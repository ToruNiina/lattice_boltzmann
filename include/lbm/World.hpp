#ifndef LATTICE_BOLTZMANN_WORLD_HPP
#define LATTICE_BOLTZMANN_WORLD_HPP

#include "BGK.hpp"
#include "Grid.hpp"
#include "Vector.hpp"

#include <vector>
#include <optional>
#include <cstdint>

namespace lbm
{

struct World
{
   public:

    World(std::int32_t nx, std::int32_t ny, BGK bgk)
        : nx_(nx), ny_(ny), grids_(nx*ny), buffer_(nx*ny),
          density_(nx*ny), velocity_(nx*ny), bgk_(bgk)
    {}

    template<typename T>
    void set_grid(std::int32_t x, std::int32_t y, T g)
    {
        const auto idx = idx_of(x,y);
        assert(idx.has_value());
        this->buffer_.at(idx.value()) = g;
        this->grids_ .at(idx.value()) = g;
    }

    void initialize(std::int32_t x, std::int32_t y, double rho, Vector u)
    {
        const auto idx = idx_of(x,y);
        assert(idx.has_value());
        this->density_ .at(idx.value()) = rho;
        this->velocity_.at(idx.value()) = u;
        this->grids_   .at(idx.value()).initialize(this->bgk_, rho, u);
    }

    void step()
    {
        // collide
        for(std::size_t i=0; i<grids_.size(); ++i)
        {
            bgk_.collide(this->grids_.at(i),
                    this->density_.at(i), this->velocity_.at(i));
        }

        // stream
        for(std::int32_t y=0; y<ny_; ++y)
        {
            for(std::int32_t x=0; x<nx_; ++x)
            {
                auto& grid = this->grids_.at(idx_of(x, y).value());

                for(const auto dir : all_dirs)
                {
                    const auto [dx, dy] = offset(dir);
                    if(const auto idx = idx_of(x+dx, y+dy))
                    {
                        this->buffer_.at(idx.value()).set_distribution(
                                dir, grid.distribution(dir));
                    }
                }
            }
        }

        // bounce back
        for(std::int32_t y=0; y<ny_; ++y)
        {
            for(std::int32_t x=0; x<nx_; ++x)
            {
                auto& grid = this->buffer_.at(idx_of(x, y).value());
                if( ! grid.bounces()) {continue;}

                for(const auto dir : all_dirs)
                {
                    const auto [back, d] = grid.bounce_back(dir);
                    const auto [dx, dy] = offset(back);

                    if(const auto back_idx = idx_of(x+dx, y+dy))
                    {
                        this->buffer_.at(back_idx.value()).set_distribution(back, d);
                    }
                }
            }
        }

        std::swap(this->buffer_, this->grids_);

        // update grid, rho, u
        for(std::size_t i=0; i<grids_.size(); ++i)
        {
            const auto rho = grids_.at(i).density();
            density_ .at(i) = rho;
            velocity_.at(i) = grids_.at(i).velocity(rho);
        }
        return;
    }

    Grid const& at(std::int32_t x, std::int32_t y) const { return grids_.at(idx_of(x,y).value()); }
    Grid&       at(std::int32_t x, std::int32_t y)       { return grids_.at(idx_of(x,y).value()); }

    double density_at (std::int32_t x, std::int32_t y) const { return density_ .at(idx_of(x,y).value()); }
    Vector velocity_at(std::int32_t x, std::int32_t y) const { return velocity_.at(idx_of(x,y).value()); }

    std::int32_t size_x() const noexcept {return nx_;}
    std::int32_t size_y() const noexcept {return ny_;}

    double rot_z(std::int32_t x, std::int32_t y) const
    {
        const auto x_pos = idx_of(x+1, y); if(!x_pos.has_value()) {return 0;}
        const auto x_neg = idx_of(x-1, y); if(!x_neg.has_value()) {return 0;}
        const auto y_pos = idx_of(x, y+1); if(!y_pos.has_value()) {return 0;}
        const auto y_neg = idx_of(x, y-1); if(!y_neg.has_value()) {return 0;}

        const auto dx = this->velocity_.at(y_pos.value()).x - this->velocity_.at(y_neg.value()).x;
        const auto dy = this->velocity_.at(x_pos.value()).y - this->velocity_.at(x_neg.value()).y;
        return (dy - dx) * 0.5;
    }

  private:

    std::optional<std::size_t> idx_of(std::int32_t x, std::int32_t y) const
    {
        if(x < 0 || nx_ <= x) {return std::nullopt;}
        if(y < 0 || ny_ <= y) {return std::nullopt;}
        return y * nx_ + x;
    }

  private:

    std::int32_t nx_;
    std::int32_t ny_;
    std::vector<Grid>   grids_;
    std::vector<Grid>   buffer_;
    std::vector<double> density_;
    std::vector<Vector> velocity_;
    BGK bgk_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_WINDOW_HPP
