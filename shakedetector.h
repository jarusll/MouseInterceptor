#ifndef SHAKEDETECTOR_H
#define SHAKEDETECTOR_H

#include <chrono>
#include <cstddef>

class ShakeDetector {
public:
    explicit ShakeDetector(int minDelta = 2,
                           size_t reversalsRequired = 4,
                           std::chrono::milliseconds window = std::chrono::milliseconds(500));

    bool feed(int dx);

private:
    const int minDelta;
    const size_t reversalsRequired;
    const std::chrono::milliseconds window;

    int lastSign = 0;
    size_t reversalCount = 0;
    bool windowActive = false;
    std::chrono::steady_clock::time_point windowStart;
};

#endif // SHAKEDETECTOR_H