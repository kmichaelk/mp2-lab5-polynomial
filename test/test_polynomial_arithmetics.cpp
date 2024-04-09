#include <gtest.h>
#include "polynomial.h"
#include <optional>
#include <unordered_map>

TEST(Polynomial, can_negate_polynomial)
{
    Polynomial p("xyz - x^2yz");

    Polynomial negated = -p;
    Polynomial expected("-xyz + x^2yz");

    EXPECT_EQ(expected, negated);
}

TEST(Polynomial, can_add_polynomials)
{
    Polynomial p1("xyz"), p2("x^2yz");
    Polynomial expected("xyz + x^2yz");

    EXPECT_EQ(expected, p1 + p2);
}

TEST(Polynomial, can_assignment_add_polynomials)
{
    Polynomial p1("xyz"), p2("x^2yz");
    Polynomial expected("xyz + x^2yz");

    ASSERT_NO_THROW(p1 += p2);

    EXPECT_EQ(expected, p1);
}

TEST(Polynomial, similar_monomials_get_compacted)
{
    Polynomial p1("xyz"), p2("xyz");
    Polynomial expected("2xyz");

    EXPECT_EQ(expected, p1 + p2);
}

//

TEST(Polynomial, can_multiply_polynomials)
{
    Polynomial p1("xyz"), p2("xyz");
    Polynomial expected("x^2y^2z^2");

    EXPECT_EQ(expected, p1 * p2);
}

TEST(Polynomial, can_assignment_multiply_polynomials)
{
    Polynomial p1("xyz"), p2("xyz");
    Polynomial expected("x^2y^2z^2");

    ASSERT_NO_THROW(p1 *= p2);

    EXPECT_EQ(expected, p1);
}

TEST(Polynomial, can_differentiate)
{
    const Polynomial m("10x^3y^4z^5 + x^2");
    const Polynomial derivativeX = m.differentiate('x');

    EXPECT_EQ(Polynomial("30x^2y^4z^5 + 2x"), derivativeX);
}

TEST(Polynomial, can_integrate)
{
    const Polynomial m("12x^3y^4z^5 + 3x^2");
    const Polynomial antiderivativeX = m.integrate('x');

    EXPECT_EQ(Polynomial("3x^4y^4z^5 + x^3"), antiderivativeX);
}
