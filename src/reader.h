#ifndef __READER_H__
#define __READER_H__

#include <sstream>

class MonomialReader
{
protected:
    MonomialReader() = default;
public:
    char peek();
    void skip();

    bool read_char(char& c);
    bool read_int(int& i);
    bool read_uint(unsigned int& u);
    bool read_double(double& d);

    void clr_err();
};

class StringMonomialReader : public MonomialReader
{
private:
    std::istringstream iss;

public:
    explicit StringMonomialReader(const std::string& input);

    char peek();
    void skip();

    bool read_char(char& c);
    bool read_int(int& i);
    bool read_uint(unsigned int& u);
    bool read_double(double& d);

    void clr_err();
};

class PStrMonomialReader : public MonomialReader
{
private:
    const char*  p;

public:
    explicit PStrMonomialReader(const char* input);

    char peek();
    void skip();

    bool read_char(char& c);
    bool read_int(int& i);
    bool read_uint(unsigned int& u);
    bool read_double(double& d);

    void clr_err();
};

#endif // __READER_H__
