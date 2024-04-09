#include "polynomial.h"

#include <stdexcept>
#include <cmath>

#include "reader.h"

inline bool verify_degree(int deg)
{
    return (deg >= Monomial::DEGREE_MIN) && (deg <= Monomial::DEGREE_MAX);
}

inline bool verify_variable(char var)
{
    return (var >= Monomial::VAR_MIN) && (var <= Monomial::VAR_MAX);
}

Monomial::Monomial(double k, Monomial::Degrees degs)
    : k(k)
    , degs(degs)
{}

Monomial::Monomial(double k)
    : k(k)
    , degs({ 0 })
{}

template<class Reader>
void Monomial::parse(Reader& reader)
{
    static_assert(std::is_base_of<MonomialReader, Reader>::value, "Reader should inherit from MonomialReader");

    const int sign = reader.peek() == '-' ? -1 : 1;
    if (reader.peek() == '-' || reader.peek() == '+')
    {
        reader.skip();
    }

    while (reader.peek() == ' ')
    {
        reader.skip();
    }

    if (!reader.read_double(k))
    {
        reader.clr_err();
        k = 1;
    }
    k *= sign;

    char var;
    int deg;

    while (reader.read_char(var))
    {
        if (var == ' ')
            continue;

        if (var < VAR_MIN || var > VAR_MAX)
        {
            throw expression_parse_error("Unsupported monomial variable");
        }

        if (reader.peek() == '^')
        {
            reader.skip();

            if (!reader.read_int(deg) || deg < DEGREE_MIN || deg > DEGREE_MAX)
            {
                throw expression_parse_error("Invalid monomial component degree");
            }
        }
        else
        {
            deg = 1;
        }

        degs[var] = deg;
    }
}

Monomial::Monomial(double k, int degX, int degY, int degZ)
    : k(k)
    , degs({ 0 })
{
    if (!verify_degree(degX) || !verify_degree(degY) || !verify_degree(degZ)) {
        throw std::invalid_argument("Degrees are out of range");
    }

    degs['x'] = static_cast<Degrees::value_t>(degX);
    degs['y'] = static_cast<Degrees::value_t>(degY);
    degs['z'] = static_cast<Degrees::value_t>(degZ);
}

Monomial::Monomial(const char* raw)
    : k(0)
    , degs({ 0 })
{
    auto parser = PStrMonomialReader(raw);
    parse(parser);
}

Monomial::Monomial(const std::string &raw)
    : k(0)
    , degs({ 0 })
{
    auto parser = StringMonomialReader(raw);
    parse(parser);
}

double Monomial::coefficient() const noexcept
{
    return k;
}

void Monomial::set_coefficient(double coefficient) noexcept
{
    k = coefficient;
}

bool Monomial::cmp_degs(const Monomial& other) const noexcept
{
    return degs == other.degs;
}

Monomial::Degrees::value_t& Monomial::operator[](char var) noexcept
{
    return const_cast<Monomial::Degrees::value_t&>(std::as_const(*this)[var]);
}
const Monomial::Degrees::value_t& Monomial::operator[](char var) const noexcept
{
    return degs[var];
}

double Monomial::calculate(const Point& point) const
{
    if (k == 0.0) {
        return 0;
    }

    double res = k;

    Degrees::value_t deg;
    for (char var = VAR_MIN; var <= VAR_MAX; var++)
    {
        deg = degs[var];

        if (deg != 0 && point.count(var) == 0)
            throw std::invalid_argument("Incomplete point, missing a component");

        res *= pow(point.at(var), deg);
    }

    return res;
}

Monomial Monomial::differentiate(char var) const
{
    if (!verify_variable(var)) {
        throw std::invalid_argument("Non-existent variable");
    }

    Monomial m(*this);
    m.k *= m.degs[var]--;

    return m;
}

Monomial Monomial::integrate(char var) const
{
    if (!verify_variable(var)) {
        throw std::invalid_argument("Non-existent variable");
    }

    Monomial m(*this);
    m.degs[var]++;
    m.k *= 1.0 / (m.degs[var]);

    return m;
}

bool Monomial::operator<(const Monomial& other) const
{
    return degs < other.degs;
}

bool Monomial::operator>(const Monomial& other) const
{
    return degs > other.degs;
}

bool Monomial::operator==(const Monomial& other) const
{
    return (k == other.k) && (degs == other.degs);
}

bool Monomial::operator!=(const Monomial& other) const
{
    return !(*this == other);
}

// region Arithmetic

Monomial Monomial::operator-() const
{
    Monomial res(*this);
    res.k *= -1;

    return res;
}

//

Monomial Monomial::operator+(const Monomial &other) const
{
    return apply_sum(*this, other, std::plus{});
}
Monomial& Monomial::operator+=(const Monomial& other)
{
    apply_sum_to(*this, other, std::plus{});
    return *this;
}

//

Monomial Monomial::operator-(const Monomial &other) const
{
    return apply_sum(*this, other, std::minus{});
}
Monomial& Monomial::operator-=(const Monomial& other)
{
    apply_sum_to(*this, other, std::minus{});
    return *this;
}

//

Monomial Monomial::operator*(const Monomial &other) const
{
    return apply_mult(*this, other, std::multiplies{}, std::plus{});
}
Monomial& Monomial::operator*=(const Monomial& other)
{
    apply_mult_to(*this, other, std::multiplies{}, std::plus{});
    return *this;
}

//

Monomial Monomial::operator/(const Monomial &other) const
{
    return apply_mult(*this, other, std::divides{}, std::minus{});
}
Monomial& Monomial::operator/=(const Monomial& other)
{
    apply_mult_to(*this, other, std::divides{}, std::minus{});
    return *this;
}

// endregion

// region Arithmetic Helpers

template<typename Operation>
Monomial Monomial::apply_sum(const Monomial& m1, const Monomial& m2, Operation op)
{
    if (m1.degs != m2.degs)
        throw std::invalid_argument("Monomial degrees does not match");
    Monomial dst(m1);
    apply_sum_to(dst, m2, op);
    return dst;
}

template<typename Operation>
void Monomial::apply_sum_to(Monomial& dst, const Monomial& other, Operation op)
{
    if (dst.degs != other.degs)
        throw std::invalid_argument("Monomial degrees does not match");
    dst.k = op(dst.k, other.k);
}

//

template<typename OperationCoefficient, typename OperationDegree>
Monomial Monomial::apply_mult(const Monomial& m1, const Monomial& m2, OperationCoefficient opK, OperationDegree opDeg)
{
    Monomial dst(m1);
    apply_mult_to(dst, m2, opK, opDeg);
    return dst;
}

template<typename OperationCoefficient, typename OperationDegree>
void Monomial::apply_mult_to(Monomial& dst, const Monomial& other, OperationCoefficient opK, OperationDegree opDeg)
{
    int result;
    for (char var = VAR_MIN; var <= VAR_MAX; var++) {
        result = opDeg(dst.degs[var], other.degs[var]);
        if (result > DEGREE_MAX) {
            throw std::runtime_error("Degree overflow");
        }
        dst.degs[var] = result;
    }
    dst.k = opK(dst.k, other.k);
}

// endregion