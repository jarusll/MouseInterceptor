#include <cstdlib>

#include "shakedetector.h"

ShakeDetector::ShakeDetector(int minDelta, size_t reversalsRequired,
                             std::chrono::milliseconds window)
    : minDelta(minDelta), reversalsRequired(reversalsRequired), window(window) {}

bool ShakeDetector::feed(int dx) {
    const auto now = std::chrono::steady_clock::now();

    if (windowActive && now - windowStart > window) {
        windowActive = false;
        reversalCount = 0;
        lastSign = 0;
    }

    if (std::abs(dx) < minDelta)
        return false;

    const int sign = dx > 0 ? 1 : -1;
    if (lastSign == 0) {
        lastSign = sign;
        return false;
    }
    if (sign == lastSign)
        return false;

    if (!windowActive) {
        windowActive = true;
        windowStart = now;
        reversalCount = 0;
    }

    ++reversalCount;
    lastSign = sign;

    if (reversalCount >= reversalsRequired) {
        windowActive = false;
        reversalCount = 0;
        lastSign = 0;
        return true;
    }

    return false;
}