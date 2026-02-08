#ifndef LATTICE_BOLTZMANN_DIRECTION_HPP
#define LATTICE_BOLTZMANN_DIRECTION_HPP

#include <array>

namespace lbm
{

enum class Direction : std::size_t
{
    Self      = 0,
    Right     = 1,
    RightUp   = 2,
    Up        = 3,
    LeftUp    = 4,
    Left      = 5,
    LeftDown  = 6,
    Down      = 7,
    RightDown = 8,
};

inline constexpr std::array<Direction, 9> all_dirs{{
    Direction::Self,
    Direction::Right    ,
    Direction::RightUp  ,
    Direction::Up       ,
    Direction::LeftUp   ,
    Direction::Left     ,
    Direction::LeftDown ,
    Direction::Down     ,
    Direction::RightDown
}};

} // lbm
#endif // LATTICE_BOLTZMANN_DIRECTION_HPP
