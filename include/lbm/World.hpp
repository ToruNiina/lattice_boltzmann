#ifndef LATTICE_BOLTZMANN_WORLD_HPP
#define LATTICE_BOLTZMANN_WORLD_HPP

#include "BGK.hpp"
#include "Grid.hpp"
#include "Vector.hpp"

#include <vector>
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
        this->buffer_.at(idx) = g;
        this->grids_ .at(idx) = g;
    }

    void initialize(std::int32_t x, std::int32_t y, double rho, Vector u)
    {
        const auto idx = idx_of(x,y);
        this->density_ .at(idx) = rho;
        this->velocity_.at(idx) = u;
        this->grids_   .at(idx).initialize(this->bgk_, rho, u);
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
                const auto idx = idx_of(x, y);
                for(const auto dir : all_dirs)
                {
                    const auto [dx, dy] = offset(dir);

                    this->buffer_.at(idx_of(x+dx, y+dy)).distribution(dir) =
                        this->grids_.at(idx).distribution(dir);
                }
            }
        }

        // bounce
        for(std::int32_t y=0; y<ny_; ++y)
        {
            for(std::int32_t x=0; x<nx_; ++x)
            {
                const auto idx = idx_of(x, y);
                if(this->buffer_.at(idx).is_barrier())
                {
                    for(const auto dir : all_dirs)
                    {
                        const auto back = bounce_back(dir);
                        const auto [dx, dy] = offset(back);

                        this->buffer_.at(idx_of(x+dx, y+dy)).distribution(back) =
                            this->buffer_.at(idx).distribution(dir);
                    }
                }
            }
        }

        // update grid, rho, u
        std::swap(this->buffer_, this->grids_);

        for(std::size_t i=0; i<grids_.size(); ++i)
        {
            const auto rho = grids_.at(i).density();
            density_ .at(i) = rho;
            velocity_.at(i) = grids_.at(i).velocity(rho);
        }
        return;
    }

    Grid const& at(std::int32_t x, std::int32_t y) const { return grids_.at(idx_of(x,y)); }
    Grid&       at(std::int32_t x, std::int32_t y)       { return grids_.at(idx_of(x,y)); }

    double density_at (std::int32_t x, std::int32_t y) const { return density_ .at(idx_of(x,y)); }
    Vector velocity_at(std::int32_t x, std::int32_t y) const { return velocity_.at(idx_of(x,y)); }

    std::int32_t size_x() const noexcept {return nx_;}
    std::int32_t size_y() const noexcept {return ny_;}

    double rot_z(std::int32_t x, std::int32_t y) const
    {
        const auto dx = this->velocity_.at(idx_of(x, y+1)).x - this->velocity_.at(idx_of(x, y-1)).x;
        const auto dy = this->velocity_.at(idx_of(x+1, y)).y - this->velocity_.at(idx_of(x-1, y)).y;
        return (dy - dx) * 0.5;
    }

  private:

    std::size_t idx_of(std::int32_t x, std::int32_t y) const
    {
        if(x < 0) {x += nx_;} else if (nx_ <= x){x -= nx_;}
        if(y < 0) {y += ny_;} else if (ny_ <= y){y -= ny_;}
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
