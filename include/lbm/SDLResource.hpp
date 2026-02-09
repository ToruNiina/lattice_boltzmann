#ifndef LATTICE_BOLTZMANN_SDL_RESOURCE_HPP
#define LATTICE_BOLTZMANN_SDL_RESOURCE_HPP

#include <format>
#include <stdexcept>

#include <SDL.h>

namespace lbm
{

struct SDLResource
{
    SDLResource(): should_quit_(true)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            throw std::runtime_error(std::format(
                        "Error while initializing SDL: {}", SDL_GetError()));
        }
    }
    ~SDLResource()
    {
        if(should_quit_)
        {
            SDL_Quit();
        }
    }

    SDLResource(const SDLResource&) = delete;
    SDLResource& operator=(const SDLResource&) = delete;

    SDLResource(SDLResource&& other): should_quit_(true)
    {
        other.should_quit_ = false;
    }
    SDLResource& operator=(SDLResource&& other)
    {
        this->should_quit_ = true;
        other.should_quit_ = false;
        return *this;
    }

  private:
    bool should_quit_;
};

} // lbm
#endif //LATTICE_BOLTZMANN_SDL_RESOURCE_HPP
