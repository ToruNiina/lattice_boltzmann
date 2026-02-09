#ifndef LATTICE_BOLTZMANN_DIRECTION_HPP
#define LATTICE_BOLTZMANN_DIRECTION_HPP

#include "Vector.hpp"
#include <array>
#include <cstdint>

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

Vector velocity_of(Direction d)
{
    using enum Direction;
    switch(d)
    {
        case Self     : { return Vector( 0, 0); }

        case Right    : { return Vector( 1, 0); }
        case Up       : { return Vector( 0, 1); }
        case Left     : { return Vector(-1, 0); }
        case Down     : { return Vector( 0,-1); }

        case RightUp  : { return Vector( 1, 1); }
        case LeftUp   : { return Vector(-1, 1); }
        case LeftDown : { return Vector(-1,-1); }
        case RightDown: { return Vector( 1,-1); }
        default: break;
    }
    return Vector(0, 0);
}

inline std::pair<std::int32_t, std::int32_t> offset(const Direction d)
{
    using enum Direction;
    switch(d)
    {
        case Self     : { return std::make_pair( 0, 0); }

        case Right    : { return std::make_pair( 1, 0); }
        case Up       : { return std::make_pair( 0, 1); }
        case Left     : { return std::make_pair(-1, 0); }
        case Down     : { return std::make_pair( 0,-1); }

        case RightUp  : { return std::make_pair( 1, 1); }
        case LeftUp   : { return std::make_pair(-1, 1); }
        case LeftDown : { return std::make_pair(-1,-1); }
        case RightDown: { return std::make_pair( 1,-1); }
        default: break;
    }
    return std::make_pair( 0, 0);
}

inline Direction bounce_back(const Direction d)
{
    using enum Direction;
    switch(d)
    {
        case Self     : { return Self;  }

        case Right    : { return Left;  }
        case Up       : { return Down;  }
        case Left     : { return Right; }
        case Down     : { return Up;    }

        case RightUp  : { return LeftDown;  }
        case LeftUp   : { return RightDown; }
        case LeftDown : { return RightUp;   }
        case RightDown: { return LeftUp;    }
        default: break;
    }
    return Self;
}

} // lbm
#endif // LATTICE_BOLTZMANN_DIRECTION_HPP
