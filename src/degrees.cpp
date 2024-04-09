#include "polynomial.h"

Monomial::Degrees::value_t& Monomial::Degrees::operator[](char var)
{
    return const_cast<value_t&>(std::as_const(*this)[var]);
}

const Monomial::Degrees::value_t& Monomial::Degrees::operator[](char var) const
{
    return values[VAR_MAX - var];
}

bool Monomial::Degrees::operator==(const Monomial::Degrees& other) const
{
    return packed == other.packed;
}

bool Monomial::Degrees::operator !=(const Monomial::Degrees& other) const
{
    return !(*this == other);
}

bool Monomial::Degrees::operator<(const Monomial::Degrees& other) const
{
    return packed < other.packed;
}

bool Monomial::Degrees::operator>(const Monomial::Degrees& other) const
{
    return packed > other.packed;
}
