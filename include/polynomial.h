#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>

#include "ordered_linkedlist.h"

class expression_parse_error : public std::runtime_error
{
public:
    explicit expression_parse_error(const std::string &message) : runtime_error(message) {}
};

class Monomial {
private:
    static const size_t COMPONENTS = 3;

    union Degrees {
        typedef char value_t;

        value_t values[COMPONENTS];
        unsigned int packed;

        value_t& operator[](char var);
        const value_t& operator[](char var) const;

        bool operator==(const Degrees& other) const;
        bool operator!=(const Degrees& other) const;

        bool operator<(const Degrees& other) const;
        bool operator>(const Degrees& other) const;
    };

    double k;
    Degrees degs;

    template<class Reader>
    void parse(Reader& reader);

    template<typename Operation>
    static Monomial apply_sum(const Monomial& m1, const Monomial& m2, Operation op);
    template<typename Operation>
    static void apply_sum_to(Monomial& dst, const Monomial& other, Operation op);
    //
    template<typename OperationCoefficient, typename OperationDegree>
    static Monomial apply_mult(const Monomial& m1, const Monomial& m2, OperationCoefficient opK, OperationDegree opDeg);
    template<typename OperationCoefficient, typename OperationDegree>
    static void apply_mult_to(Monomial& dst, const Monomial& other, OperationCoefficient opK, OperationDegree opDeg);

    Monomial(double k, Degrees degs);
public:
    static const char VAR_MAX = 'z';
    static const char VAR_MIN = VAR_MAX - COMPONENTS + 1;

    typedef std::unordered_map<char, double> Point;

    static const int DEGREE_MIN = std::numeric_limits<Degrees::value_t>::min();
    static const int DEGREE_MAX = std::numeric_limits<Degrees::value_t>::max();
    // static const int DEGREE_MAX = std::numeric_limits<decltype(*(static_cast<Degrees*>(nullptr)->values))>::max();

    explicit Monomial(const std::string& raw);
    explicit Monomial(const char* raw);
    Monomial(double k, int degX, int degY, int degZ);
    explicit Monomial(double k);

    [[nodiscard]]
    double coefficient() const noexcept;
    void set_coefficient(double coefficient) noexcept;

    [[nodiscard]]
    bool cmp_degs(const Monomial& other) const noexcept;

    Degrees::value_t& operator[](char var) noexcept;
    const Degrees::value_t& operator[](char var) const noexcept;

    [[nodiscard]]
    double calculate(const Point& point) const;

    [[nodiscard]] Monomial differentiate(char variable) const;
    [[nodiscard]] Monomial integrate(char variable) const;

    bool operator<(const Monomial& other) const;
    bool operator>(const Monomial& other) const;
    bool operator==(const Monomial& other) const;
    bool operator!=(const Monomial& other) const;

    Monomial operator-() const;
    //
    Monomial operator+(const Monomial& other) const;
    Monomial& operator+=(const Monomial& other);
    //
    Monomial operator-(const Monomial& other) const;
    Monomial& operator-=(const Monomial& other);
    //
    Monomial operator*(const Monomial& other) const;
    Monomial& operator*=(const Monomial& other);
    //
    Monomial operator/(const Monomial& other) const;
    Monomial& operator/=(const Monomial& other);

    friend std::ostream& operator<<(std::ostream& os, const Monomial& m)
    {
        if (fabs(m.k) != 1.0) {
            os << m.k;
        }

        Degrees::value_t deg;
        for (char var = VAR_MIN; var <= VAR_MAX; var++)
        {
            deg = m.degs[var];
            if (deg == 0)
                continue;

            os << var;
            if (deg != 1) {
                os << "^" << static_cast<int>(deg);
            }
        }

        return os;
    }
};

class Polynomial {
private:
    static bool OrderFunction(const Monomial& a, const Monomial& b);

    OrderedLinkedList<Monomial> monomials = OrderedLinkedList<Monomial>(OrderFunction);

    template<typename Operation>
    static Polynomial apply_sum(const Polynomial& p1, const Polynomial& p2, Operation op);
    template<typename Operation>
    static Polynomial apply_mult(const Polynomial& p1, const Polynomial& p2, Operation op);

public:

    Polynomial();
    Polynomial(Monomial monomial);
    explicit Polynomial(const std::string& raw);

    void insert(const Monomial& monomial);
    const Monomial& operator[](size_t idx) const;
    [[nodiscard]] size_t size() const;

    void compact();

    [[nodiscard]]
    double calculate(const Monomial::Point& point) const;

    [[nodiscard]] Polynomial differentiate(char variable) const;
    [[nodiscard]] Polynomial integrate(char variable) const;

    bool operator==(const Polynomial& other) const;
    bool operator!=(const Polynomial& other) const;

    Polynomial operator-() const;
    //
    Polynomial operator+(const Polynomial& other) const;
    Polynomial& operator+=(const Polynomial& other);
    //
    Polynomial operator-(const Polynomial& other) const;
    Polynomial& operator-=(const Polynomial& other);
    //
    Polynomial operator*(const Polynomial& other) const;
    Polynomial& operator*=(const Polynomial& other);
    //
    Polynomial operator/(const Polynomial& other) const;
    Polynomial& operator/=(const Polynomial& other);

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p)
    {
        double k, kAbs;
        std::remove_reference<decltype(static_cast<Monomial*>(nullptr)->operator[](0))>::type deg;
        bool fst = true;

        // for (auto it = p.monomials.cbegin(); it != p.monomials.cend(); ++it)
        for (const auto& m : std::as_const(p.monomials))
        {
            k = m.coefficient();
            kAbs = fabs(k);

            os << (fst ? "" : " ") << ((k < 0) ? "-" : (fst ? "" : "+")) << (fst ? "" : " ");
            if (kAbs != 1.0f) {
                os << kAbs;
            }

            for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++)
            {
                deg = m[var];
                if (deg == 0)
                    continue;

                os << var;
                if (deg != 1) {
                    os << "^" << static_cast<int>(deg);
                }
            }

            fst = false;
        }

        return os;
    }
};


#endif // __POLYNOMIAL_H__
