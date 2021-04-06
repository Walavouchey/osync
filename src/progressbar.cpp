#include <progressbar.hpp>

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

ProgressBar::ProgressBar(std::string prefix, int denominator, int precision)
    :
    prefix(prefix),
    denominator(denominator),
    precision(precision)
{
    print();
    start = std::chrono::steady_clock::now();
}

std::string formatTime(const double t)
{
    int ms = t * 1000;
    int h = ms / (1000 * 60 * 60);
    ms -= h * (1000 * 60 * 60);
    int m = ms / (1000 * 60);
    ms -= m * (1000 * 60);
    int s = ms / 1000;
    ms -= s * 1000;
    return (std::ostringstream() << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2) << m
        << ':' << std::setw(2) << s << '.' << std::setw(3) << ms).str();
}

void ProgressBar::print()
{
    now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - start;
    std::cout << prefix << numerator << '/' << denominator << ", "
        << std::setprecision(precision) << std::fixed
        << 100 * numerator / (float) denominator << "%, elapsed: "
        << formatTime(elapsed.count()) << ", remaining: ";
    if (numerator != 0) std::cout << formatTime(elapsed.count() * (denominator / (float) numerator - 1));
    std::cout << "\r";
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
