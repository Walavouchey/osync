#include <progressbar.hpp>

#include <strext.hpp>

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
    start = std::chrono::steady_clock::now();
    if (this->reportInterval.count() >= 0)
    {
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

void ProgressBar::print()
{
    now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - start;
    std::cout << (std::ostringstream() << '\r' << prefix << numerator << '/' << denominator << ", "
        << std::setprecision(precision) << std::fixed << 100 * numerator / (float) denominator << '%'
        << (!reportTime ? "" : (std::ostringstream() << ", elapsed: "
        << strext::formatTime(elapsed.count()) << ", remaining: "
        << (numerator == 0 ? "" : strext::formatTime(elapsed.count() * (denominator / (float) numerator - 1)))).str())).str();
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
