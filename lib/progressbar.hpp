#pragma once

#include <string>

class ProgressBar
{
public:
    ProgressBar(std::string prefix, int denominator, int precision = 0);
    void print();
    void finish();
    ~ProgressBar();
private:
    std::string prefix;
    int numerator = 0;
    int denominator;
    int precision;
    bool done = false;
};
