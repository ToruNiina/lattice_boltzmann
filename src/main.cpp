#include <lbm/World.hpp>
#include <lbm/Window.hpp>

#include <thread>

int main()
{
    lbm::BGK model(0.02);
    lbm::World world(200, 80, model);

    const auto init_rho = 1.0;
    const auto init_vel = lbm::Vector(0.1, 0.0);
    for(std::int32_t y=0; y<world.size_y(); ++y)
    {
        for(std::int32_t x=0; x<world.size_x(); ++x)
        {
            world.initialize(x, y, init_rho, init_vel);
        }
    }
    {
        for(std::int32_t y=world.size_y()*0.4; y<world.size_y()*0.55; ++y)
        {
            world.set_grid(world.size_x()*0.2, y, lbm::Barrier());
        }

        lbm::ConstantFlow boundary;
        boundary.initialize(model, init_rho, init_vel);

        for(std::int32_t y=0; y<world.size_y(); ++y)
        {
            world.set_grid(0,                y, boundary);
            world.set_grid(world.size_x()-1, y, boundary);
        }
        for(std::int32_t x=0; x<world.size_x(); ++x)
        {
            world.set_grid(x, 0               , boundary);
            world.set_grid(x, world.size_y()-1, boundary);
        }
    }

    lbm::Window window(200, 80, 4);
    while( ! window.finish())
    {
        window.update(world);
        for(std::size_t i=0; i<20; ++i)
        {
            world.step();
        }
    }
    return 0;
}
