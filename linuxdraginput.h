#ifndef LINUXDRAGINPUT_H
#define LINUXDRAGINPUT_H

#include <functional>
#include <linux/input.h>

#include "DragInputSource.h"

class LinuxDragInput : public DragInput {
public:
    LinuxDragInput();
    ~LinuxDragInput() override;

    void onDrag(std::function<void(int dx, int dy)> callback) override;
    void run() override;

private:
    int fd = -1;
    struct input_event buffer[64];
    std::function<void(int dx, int dy)> callback;
};

#endif // LINUXDRAGINPUT_H
