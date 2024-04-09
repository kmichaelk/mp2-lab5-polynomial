#include <gtest.h>
#include "polynomial.h"

TEST(Polynomial, can_parse_signed)
{
    Polynomial p("-32x^10z^50 + 90x^5y^10z^15");

    ASSERT_EQ(2, p.size());
    EXPECT_EQ(Monomial("-32x^10z^50"), p[0]);
    EXPECT_EQ(Monomial("90x^5y^10z^15"), p[1]);
}

TEST(Polynomial, can_parse_unsigned)
{
    Polynomial p("32x^10z^50 + 90x^5y^10z^15");

    ASSERT_EQ(2, p.size());
    EXPECT_EQ(Monomial("32x^10z^50"), p[0]);
    EXPECT_EQ(Monomial("90x^5y^10z^15"), p[1]);
}

TEST(Polynomial, can_parse_single)
{
    Polynomial p("-90x^5y^10z^15");

    ASSERT_EQ(1, p.size());
    EXPECT_EQ(Monomial("-90x^5y^10z^15"), p[0]);
}

TEST(Polynomial, can_parse_single_unsigned)
{
    Polynomial p("90x^5y^10z^15");

    ASSERT_EQ(1, p.size());
    EXPECT_EQ(Monomial("90x^5y^10z^15"), p[0]);
}

TEST(Polynomial, monomials_are_sorted_by_descending_degrees)
{
    Polynomial p("5xyz^3 + xyz + 15x^2yz + 10x^2y^2z");

    ASSERT_EQ(4, p.size());
    EXPECT_EQ(Monomial("10x^2y^2z"), p[0]);
    EXPECT_EQ(Monomial("15x^2yz"), p[1]);
    EXPECT_EQ(Monomial("5xyz^3"), p[2]);
    EXPECT_EQ(Monomial("xyz"), p[3]);
}

TEST(Polynomial, zero_is_not_being_added)
{
    Polynomial p("0xyz");

    EXPECT_EQ(0, p.size());
}

TEST(Polynomial, can_compact)
{
    Polynomial p("xyz + xyz");
    p.compact();

    ASSERT_EQ(1, p.size());
    EXPECT_EQ(Monomial("2xyz"), p[0]);
}

TEST(Polynomial, can_calculate)
{
    const std::unordered_map<char, double> point = {
            { 'x', 5 },
            { 'y', 6 },
            { 'z', 7 },
    };

    const Monomial m0(-32, 10, 0, 50), m1(90, 5, 10, 15);
    const double res = m0.calculate(point) + m1.calculate(point);

    Polynomial p;
    p.insert(m0);
    p.insert(m1);

    EXPECT_EQ(res, p.calculate(point));
}
