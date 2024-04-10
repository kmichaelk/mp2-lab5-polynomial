#include "reader.h"

// region StringMonomialReader

StringMonomialReader::StringMonomialReader(const std::string& input)
    : iss(input)
{}

char StringMonomialReader::peek()
{
    return static_cast<char>(iss.peek());
}
void StringMonomialReader::skip()
{
    iss.ignore();
}

bool StringMonomialReader::read_char(char& c)
{
    return !!(iss >> c);
}
bool StringMonomialReader::read_int(int& i)
{
    return !!(iss >> i);
}
bool StringMonomialReader::read_uint(unsigned int& u)
{
    return !!(iss >> u);
}
bool StringMonomialReader::read_double(double& d)
{
    return !!(iss >> d);
}

void StringMonomialReader::clr_err()
{
    iss.clear();
}

// endregion

// region PStrMonomialReader

PStrMonomialReader::PStrMonomialReader(const char* input)
        : p(input)
{}

char PStrMonomialReader::peek()
{
    return *p;
}
void PStrMonomialReader::skip()
{
    if (!*p)
        return;
    p++;
}

bool PStrMonomialReader::read_char(char& c)
{
    if (!*p)
        return false;

    c = *p++;
    return true;
}
bool PStrMonomialReader::read_int(int& i)
{
    int read, result;
    result = sscanf_s(p, "%d%n", &i, &read);
    if (result) {
        p += read;
    }
    return result;
}
bool PStrMonomialReader::read_uint(unsigned int& u)
{
    int read, result;
    result = sscanf_s(p, "%u%n", &u, &read);
    if (result) {
        p += read;
    }
    return result;
}
bool PStrMonomialReader::read_double(double& d)
{
    if (*p == '0' && *(p + 1) != '.') {
        skip();
        d = 0;
        return true;
    }

    int read, result;
    result = sscanf_s(p, "%lf%n", &d, &read);
    if (result) {
        p += read;
    }
    return result;
}

void PStrMonomialReader::clr_err()
{
    // NOP
}

// endregion