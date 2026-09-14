#ifndef DRAGINPUTSOURCE_H
#define DRAGINPUTSOURCE_H

#include <functional>

class DragInput {
public:
    virtual ~DragInput() = default;

    virtual void onDrag(std::function<void(int dx, int dy)> callback) = 0;
    virtual void run() = 0;
};

#endif // DRAGINPUTSOURCE_H
