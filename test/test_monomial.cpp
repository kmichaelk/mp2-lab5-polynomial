#include <gtest.h>
#include "polynomial.h"
#include <cmath>
#include <optional>

TEST(Monomial, can_initialize_explicitly)
{
    const int k = 10, degX = 1, degY = 2, degZ = 3;

    Monomial monomial(k, degX, degY, degZ);

    EXPECT_EQ(k, monomial.coefficient());
    EXPECT_EQ(degX, monomial['x']);
    EXPECT_EQ(degY, monomial['y']);
    EXPECT_EQ(degZ, monomial['z']);
}

TEST(Monomial, can_initialize_by_parsing)
{
    const int k = 10, degX = 1, degY = 2, degZ = 3;
    const std::string raw = std::to_string(k)
            + "x^" + std::to_string(degX)
            + "y^" + std::to_string(degY)
            + "z^" + std::to_string(degZ);

    Monomial monomial(raw);

    EXPECT_EQ(k, monomial.coefficient());
    EXPECT_EQ(degX, monomial['x']);
    EXPECT_EQ(degY, monomial['y']);
    EXPECT_EQ(degZ, monomial['z']);
}

TEST(Monomial, can_parse_in_any_order)
{
    Monomial monomial("y^2x^1z^3");

    EXPECT_EQ(1, monomial['x']);
    EXPECT_EQ(2, monomial['y']);
    EXPECT_EQ(3, monomial['z']);
}

TEST(Monomial, can_parse_partial_monomials)
{
    Monomial monomial("32x^10z^50");

    EXPECT_EQ(32, monomial.coefficient());
    EXPECT_EQ(10, monomial['x']);
    EXPECT_EQ(0, monomial['y']);
    EXPECT_EQ(50, monomial['z']);
}

TEST(Monomial, can_parse_fractional_coefficients)
{
    Monomial monomial("0.1xyz");

    EXPECT_EQ(0.1, monomial.coefficient());
}

TEST(Monomial, can_parse_zero_coefficient)
{
    Monomial monomial("0xyz");

    EXPECT_EQ(0, monomial.coefficient());
}

TEST(Monomial, defaults_to_coefficient_if_not_supplied)
{
    std::optional<Monomial> m;
    ASSERT_NO_THROW(m = Monomial("x^10z^50"));
    EXPECT_EQ(1, m->coefficient());
}

TEST(Monomial, defaults_to_degree_if_not_supplied)
{
    std::optional<Monomial> m;
    ASSERT_NO_THROW(m = Monomial("8xz^50"));
    EXPECT_EQ(1, (*m)['x']);
}

TEST(Monomial, fails_on_unsupported_variable)
{
    EXPECT_ANY_THROW(Monomial m("32a^10z^50"));
}

TEST(Monomial, fails_on_missing_expected_degree)
{
    EXPECT_ANY_THROW(Monomial m("32x^z^50"));
}

TEST(Monomial, can_calculate)
{
    const int k = 10, degX = 1, degY = 2, degZ = 3;
    const std::unordered_map<char, double> point = {
            { 'x', 5 },
            { 'y', 6 },
            { 'z', 7 },
    };

    const double result = k
            * pow(point.at('x'), degX)
            * pow(point.at('y'), degY)
            * pow(point.at('z'), degZ);

    Monomial monomial(k, degX, degY, degZ);

    EXPECT_EQ(result, monomial.calculate(point));
}
