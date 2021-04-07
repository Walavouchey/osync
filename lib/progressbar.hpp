#pragma once

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

class ProgressBar
{
public:
    ProgressBar(std::string prefix, int denominator, int precision = 0, float reportInterval = -1, bool reportTime = true);
    void print();
    void update();
    void finish();
    ~ProgressBar();
private:
    std::thread thread;
    std::string prefix;
    std::atomic<int> numerator = 0;
    int denominator;
    int precision;
    std::chrono::duration<float> reportInterval;
    bool reportTime;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> now;
    std::atomic<bool> done = false;
    void timer();
};
