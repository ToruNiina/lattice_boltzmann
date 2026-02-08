#ifndef LATTICE_BOLTZMANN_WINDOW_HPP
#define LATTICE_BOLTZMANN_WINDOW_HPP

#include "SDLResource.hpp"

namespace lbm
{

struct Window
{
   public:

    Window(std::size_t w, std::size_t h, std::size_t c)
    : origin_x_(0), origin_y_(0), cell_size_(c),
      resource_{},
      Window_(SDL_CreateWindow("haywire", 0, 0, w, h, SDL_Window_RESIZABLE), &SDL_DestroyWindow),
      renderer_(SDL_CreateRenderer(Window_.get(), -1, 0), &SDL_DestroyRenderer)
    {
        SDL_GetMouseState(&mouse_prev_x_, &mouse_prev_y_);

        assert(origin_x_ + w < world_.width()  * cell_size_);
        assert(origin_y_ + h < world_.height() * cell_size_);
    }
    ~Window() = default;
    Window(const Window&) = delete;
    Window(Window&&)      = default;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&)      = default;

  private:

    SDLResource resource_;
    std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>   Window_;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer_;
};

} // lbm
#endif // LATTICE_BOLTZMANN_WINDOW_HPP
