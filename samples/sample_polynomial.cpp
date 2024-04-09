#include <iostream>

#include "polynomial.h"

using namespace std;

int main()
{
    string input;

    cout << "Enter polynomial: ";
    getline(cin, input);

    Polynomial polynomial(input);
    polynomial.compact();

    cout << " Parsed: " << polynomial << endl;

    cout << " Derivatives: " << endl;
    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        cout << "  '" << var << "': " << polynomial.differentiate(var) << endl;
    }

    cout << " Antiderivatives: " << endl;
    for (char var = Monomial::VAR_MIN; var <= Monomial::VAR_MAX; var++) {
        cout << "  '" << var << "': " << polynomial.integrate(var) << endl;
    }

    return EXIT_SUCCESS;
}
