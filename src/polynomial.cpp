#include "polynomial.h"

#include <iostream>

bool Polynomial::OrderFunction(const Monomial &a, const Monomial &b)
{
    return a > b;
}

Polynomial::Polynomial() = default;

Polynomial::Polynomial(const Monomial monomial)
{
    monomials.push(monomial);
}

Polynomial::Polynomial(const std::string& raw)
{
    const size_t length = raw.length();

    // move null-terminator temporarily for extra optimization
    // and copying avoidance, using C-style strings
    size_t marker;
    char to_restore = 0;

    bool has_unparsed_data = true;

    for (size_t i = length; i-- > 0; ) {
        if (raw[i] == '+' || raw[i] == '-') {
            insert(Monomial(&raw[i]));

            if (to_restore != 0) {
                const_cast<std::string&>(raw)[marker] = to_restore;
            }
            to_restore = raw[i];
            const_cast<std::string&>(raw)[i] = 0;
            marker = i;

            has_unparsed_data = false;
        } else if (raw[i] != ' ') {
            has_unparsed_data = true;
        }
    }

    if (has_unparsed_data) {
        insert(Monomial(&raw[0]));
    }

    if (to_restore != 0) {
        const_cast<std::string&>(raw)[marker] = to_restore;
    }
}

void Polynomial::insert(const Monomial& monomial)
{
    if (monomial.coefficient() == 0.0)
        return;

    monomials.push(monomial);
}

const Monomial& Polynomial::operator[](size_t idx) const
{
    return monomials[idx];
}

size_t Polynomial::size() const
{
    return monomials.size();
}

void Polynomial::compact()
{
    OrderedLinkedList<Monomial> buf(OrderFunction);
    swap(monomials, buf);

    Monomial* cur = nullptr;
    for (auto& m : buf) {
        if (!cur) {
            cur = &m;
            continue;
        }
        if (cur->cmp_degs(m)) {
            *cur = *cur + m;
            continue;
        }
        monomials.push(*cur);
        cur = &m;
    }
    if (cur) {
        monomials.push(*cur);
    }
}

double Polynomial::calculate(const Monomial::Point& point) const
{
    double res = .0;
    for (size_t i = 0; i < monomials.size(); i++)
    {
        res += monomials[i].calculate(point);
    }
    return res;
}

Polynomial Polynomial::differentiate(char var) const
{
    Polynomial res;
    for (const auto& item : monomials) {
        res.insert(item.differentiate(var));
    }
    return res;
}

Polynomial Polynomial::integrate(char var) const
{
    Polynomial res;
    for (const auto& item : monomials) {
        res.insert(item.integrate(var));
    }
    return res;
}

bool Polynomial::operator==(const Polynomial& other) const
{
    return monomials == other.monomials;
}

bool Polynomial::operator!=(const Polynomial& other) const
{
    return !(*this == other);
}

// region Arithmetic
#include <iostream>
Polynomial Polynomial::operator-() const
{
    Polynomial res; // inserting element-by-element would be slower due to order check overhead
    for (auto& item : monomials) {
        res.insert(-item);
    }
    return res;
}

//

Polynomial Polynomial::operator+(const Polynomial& other) const
{
    return apply_sum(*this, other, 1);
}
Polynomial& Polynomial::operator+=(const Polynomial& other)
{
    *this = *this + other;
    return *this;
}

//

Polynomial Polynomial::operator-(const Polynomial& other) const
{
    return apply_sum(*this, other, -1);
}
Polynomial& Polynomial::operator-=(const Polynomial& other)
{
    *this = *this - other;
    return *this;
}

//

Polynomial Polynomial::operator*(const Polynomial& other) const
{
    return apply_mult(*this, other, std::multiplies{});
}
Polynomial& Polynomial::operator*=(const Polynomial& other)
{
    *this = *this * other;
    return *this;
}

//

Polynomial Polynomial::operator/(const Polynomial& other) const
{
    return apply_mult(*this, other, std::divides{});
}
Polynomial& Polynomial::operator/=(const Polynomial& other)
{
    *this = *this / other;
    return *this;
}

// endregion

// region Arithmetic Helpers

//
// We will use the fact that monomial list is sorted
// for matching monomials search optimization
//

//
// If we'd update existing Polynomial objects list,
// we will face a lot of overhead due to list ordering,
// so it's faster to create new objects
//

Polynomial Polynomial::apply_sum(const Polynomial& p1, const Polynomial& p2, int sign)
{
    Polynomial dst;

    const Polynomial* pMin = (p1.size() < p2.size()) ? &p1 : &p2;
    const Polynomial* pMax = (p1.size() < p2.size()) ? &p2 : &p1;

    const bool iterating_rhs = pMax == &p2;
    const Monomial mult(sign);

    auto itMin = pMin->monomials.cbegin();
    for (const auto& m : pMax->monomials) {
        if (itMin == pMin->monomials.cend() || m < *itMin) {
            dst.insert(iterating_rhs ? mult * m : m);
            continue;
        } else if (m.cmp_degs(*itMin)) {
            dst.insert(iterating_rhs ? (*itMin + mult * m) : (mult * *itMin + m));
        } else {
            dst.insert(iterating_rhs ? mult * m : m);
            continue;
        }

        ++itMin;
    }

    for (; itMin != pMin->monomials.cend(); ++itMin) {
        dst.insert(iterating_rhs ? *itMin : mult * *itMin);
    }

    return dst;
}

template<typename Operation>
Polynomial Polynomial::apply_mult(const Polynomial& p1, const Polynomial& p2, Operation op)
{
    Polynomial dst;

    for (const auto& m1 : p1.monomials) {
        for (const auto& m2 : p2.monomials) {
            dst.insert(op(m1, m2));
        }
    }

    return dst;
}

// endregion
