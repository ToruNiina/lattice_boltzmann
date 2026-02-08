#ifndef LATTICE_BOLTZMANN_VECTOR_HPP
#define LATTICE_BOLTZMANN_VECTOR_HPP

namespace lbm
{

struct Vector
{
    double x;
    double y;
};

Vector operator-(const Vector& lhs)
{
    return Vector{-lhs.x, -lhs.y};
}
Vector operator+(const Vector& lhs, const Vecotr& rhs)
{
    return Vector{lhs.x + rhs.x, lhs.y + rhs.y};
}
Vector operator-(const Vector& lhs, const Vecotr& rhs)
{
    return Vector{lhs.x - rhs.x, lhs.y - rhs.y};
}
Vector operator*(const Vector& lhs, const double rhs)
{
    return Vector{lhs.x * rhs, lhs.y * rhs};
}
Vector operator*(const double lhs, const Vecotr& rhs)
{
    return Vector{lhs * rhs.x, lhs * rhs.y};
}

double dot_product(const Vector& lhs, const Vecotr& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}
double length(const Vector& lhs)
{
    return dot_product(lhs, lhs);
}

} // lbm
#endif // LATTICE_BOLTZMANN_VECTOR_HPP
