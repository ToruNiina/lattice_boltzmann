cmake_minimum_required(VERSION 3.28)
project(lattice_boltzmann)

include(cmake/CPM.cmake)

find_package(SDL2 REQUIRED)

CPMAddPackage(
    NAME toml11
    GITHUB_REPOSITORY "ToruNiina/toml11"
    VERSION 4.4.0
    OPTIONS "TOML11_PRECOMPILE ON" "CMAKE_CXX_STANDARD 20" "TOML11_ENABLE_ACCESS_CHECK ON"
    )

add_subdirectory(src)
