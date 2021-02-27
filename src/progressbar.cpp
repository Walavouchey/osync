#include <progressbar.hpp>

#include <iomanip>
#include <iostream>

ProgressBar::ProgressBar(std::string prefix, int denominator, int precision)
    :
    prefix(prefix),
    denominator(denominator),
    precision(precision)
{
    print();
}

void ProgressBar::print()
{
    std::cout << prefix << numerator << '/' << denominator << ", "
        << std::setprecision(precision) << std::fixed
        << 100 * numerator / (float) denominator << "%\r";
    numerator++;
}

void ProgressBar::finish()
{
    std::cout << std::endl;
    done = true;
}

ProgressBar::~ProgressBar()
{
    if (!done) finish();
}
