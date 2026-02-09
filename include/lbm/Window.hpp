#ifndef LATTICE_BOLTZMANN_WINDOW_HPP
#define LATTICE_BOLTZMANN_WINDOW_HPP

#include "SDLResource.hpp"
#include "World.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

#include <iostream>

namespace lbm
{

struct Window
{
   public:

    Window(std::size_t w, std::size_t h, std::size_t c)
        : finish_(false), cell_size_(c),
          sdl_resource_{},
          window_(nullptr, &SDL_DestroyWindow),
          renderer_(nullptr, &SDL_DestroyRenderer)
    {
        const auto nw = w * c;
        const auto nh = h * c;

        const SDL_WindowFlags flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_ALLOW_HIGHDPI);

        this->window_.reset(SDL_CreateWindow("LBM",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    nw, nh, flags));
        if(window_ == nullptr)
        {
            throw std::runtime_error(std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
        }

        this->renderer_.reset(SDL_CreateRenderer(window_.get(), -1,
                    SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED));
        if(renderer_ == nullptr)
        {
            throw std::runtime_error(std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
        }
    }

    ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&)      = default;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&)      = default;

    void update(const World& w)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                finish_ = true;
            }
            else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window_.get()))
            {
                finish_ = true;
            }
        }
        if (SDL_GetWindowFlags(window_.get()) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            return;
        }

        SDL_SetRenderDrawColor(renderer_.get(), 0x80, 0x80, 0x80, 0xFF);
        SDL_RenderClear(renderer_.get());

        for(std::int32_t y=0; y<w.size_y(); ++y)
        {
            for(std::int32_t x=0; x<w.size_x(); ++x)
            {
                const SDL_Rect cell{
                    .x = x * cell_size_,
                    .y = y * cell_size_,
                    .w = cell_size_,
                    .h = cell_size_
                };

                if(w.at(x, y).is_barrier())
                {
                    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 0xFF);
                }
                else
                {
                    const auto scale = w.rot_z(x, y) * 40;
                    const auto [r, g, b] = colormap(scale);
                    SDL_SetRenderDrawColor(renderer_.get(), r, g, b, 0xFF);
                }
                SDL_RenderFillRect(renderer_.get(), std::addressof(cell));
            }
        }
        SDL_RenderPresent(renderer_.get());
        return ;
    }

    bool finish() const noexcept {return finish_;}

    std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> colormap(double x) const
    {
        if(0 < x)
        {
            const auto color = std::clamp<int>((1-std::abs(x)) * 256, 0, 255);
            return std::make_tuple(0xFF, color, color);
        }
        else if(x < 0)
        {
            const auto color = std::clamp<int>((1-std::abs(x)) * 256, 0, 255);
            return std::make_tuple(color, color, 0xFF);
        }
        else
        {
            return std::make_tuple(0xFF, 0xFF, 0xFF);
        }
    }

  private:

    bool finish_;
    std::int32_t cell_size_;
    SDLResource sdl_resource_;
    std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>   window_;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_WINDOW_HPP
