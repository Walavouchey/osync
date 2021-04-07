#include <progressbar.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>

ProgressBar::ProgressBar(std::string prefix, int denominator, int precision, float reportInterval, bool reportTime)
    :
    prefix(prefix),
    denominator(denominator),
    precision(precision),
    reportInterval(reportInterval),
    reportTime(reportTime)
{
    print();
    if (this->reportInterval.count() >= 0)
    {
        start = std::chrono::steady_clock::now();
        thread = std::thread(&ProgressBar::timer, this);
    }
}

void ProgressBar::timer()
{
    while (!done)
    {
        print();
        std::this_thread::sleep_for(reportInterval);
    }
    print();
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
    std::cout << (std::ostringstream() << '\r' << prefix << numerator << '/' << denominator << ", "
        << std::setprecision(precision) << std::fixed << 100 * numerator / (float) denominator << '%'
        << (!reportTime ? "" : (std::ostringstream() << ", elapsed: "
        << formatTime(elapsed.count()) << ", remaining: "
        << (numerator == 0 ? "" : formatTime(elapsed.count() * (denominator / (float) numerator - 1)))).str())).str();
}

void ProgressBar::update()
{
    numerator++;
    if (reportInterval.count() < 0) print();
}

void ProgressBar::finish()
{
    done = true;
    if (thread.joinable()) thread.join();
    std::cout << std::endl;
}

ProgressBar::~ProgressBar()
{
    if (!done) finish();
}
