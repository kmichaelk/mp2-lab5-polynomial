#include <gtest.h>
#include "polynomial.h"
#include <optional>
#include <unordered_map>

TEST(Monomial, can_negate_monomial)
{
    Monomial m("8x^2y^3z^4");
    const double k = m.coefficient();

    ASSERT_NO_THROW(m = -m);

    EXPECT_EQ(-k, m.coefficient());
}

TEST(Monomial, can_add_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");

    std::optional<Monomial> m;
    ASSERT_NO_THROW(m = m1 + m2);

    EXPECT_EQ(m1.coefficient() + m2.coefficient(), m->coefficient());
}

TEST(Monomial, can_assignment_add_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");
    const double k1 = m1.coefficient(), k2 = m2.coefficient();

    ASSERT_NO_THROW(m1 += m2);

    EXPECT_EQ(k1 + k2, m1.coefficient());
}

TEST(Monomial, cant_add_monomials_with_different_degrees)
{
    Monomial m1("8x^2y^3z^5"), m2("2x^2y^3z^4");

    EXPECT_ANY_THROW(Monomial m = m1 + m2);
}

//

TEST(Monomial, can_subtract_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");

    std::optional<Monomial> m;
    ASSERT_NO_THROW(m = m1 - m2);

    EXPECT_EQ(m1.coefficient() - m2.coefficient(), m->coefficient());
}

TEST(Monomial, can_assignment_subtract_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");
    const double k1 = m1.coefficient(), k2 = m2.coefficient();

    ASSERT_NO_THROW(m1 -= m2);

    EXPECT_EQ(k1 - k2, m1.coefficient());
}

TEST(Monomial, cant_subtract_monomials_with_different_degrees)
{
    Monomial m1("8x^2y^3z^5"), m2("2x^2y^3z^4");

    EXPECT_ANY_THROW(Monomial m = m1 - m2);
}

//

TEST(Monomial, can_multiply_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");

    std::optional<Monomial> m;
    ASSERT_NO_THROW(m = m1 * m2);

    EXPECT_EQ(m1.coefficient() * m2.coefficient(), m->coefficient());
    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        EXPECT_EQ(m1[var] + m2[var], (*m)[var]);
    }
}

TEST(Monomial, can_assignment_multiply_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");

    const double k1 = m1.coefficient(), k2 = m2.coefficient();
    std::unordered_map<char, unsigned char> d1, d2;

    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        d1[var] = m1[var];
        d2[var] = m2[var];
    }

    ASSERT_NO_THROW(m1 *= m2);

    EXPECT_EQ(k1 * k2, m1.coefficient());
    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        EXPECT_EQ(m1[var], d1[var] + d2[var]);
    }
}

//

TEST(Monomial, can_divide_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");

    std::optional<Monomial> m;
    ASSERT_NO_THROW(m = m1 / m2);

    EXPECT_EQ(m1.coefficient() / m2.coefficient(), m->coefficient());
    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        EXPECT_EQ(m1[var] - m2[var], (*m)[var]);
    }
}

TEST(Monomial, can_assignment_divide_monomials)
{
    Monomial m1("8x^2y^3z^4"), m2("2x^2y^3z^4");

    const double k1 = m1.coefficient(), k2 = m2.coefficient();
    std::unordered_map<char, unsigned char> d1, d2;

    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        d1[var] = m1[var];
        d2[var] = m2[var];
    }

    ASSERT_NO_THROW(m1 /= m2);

    EXPECT_EQ(k1 / k2, m1.coefficient());
    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        EXPECT_EQ(m1[var], d1[var] - d2[var]);
    }
}

TEST(Monomial, can_differentiate)
{
    const Monomial m("10x^3y^4z^5");
    const Monomial derivativeX = m.differentiate('x');

    EXPECT_EQ(Monomial("30x^2y^4z^5"), derivativeX);
}

TEST(Monomial, can_integrate)
{
    const Monomial m("12x^3y^4z^5");
    const Monomial antiderivativeX = m.integrate('x');

    EXPECT_EQ(Monomial("3x^4y^4z^5"), antiderivativeX);
}