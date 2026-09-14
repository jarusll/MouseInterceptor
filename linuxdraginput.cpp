#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <cassert>

#include <QDebug>

#include "linuxdraginput.h"

static bool has_bit(unsigned long *bits, int nr) { return bits[0] & (1UL << nr); }

LinuxDragInput::LinuxDragInput() {
    DIR *dir = opendir("/dev/input");
    if (!dir) {
        qCritical() << "Failed to open /dev/input";
        exit(1);
    }

    qInfo() << "Present input devices:";
    struct dirent *ent;
    while ((ent = readdir(dir)) != nullptr) {
        if (strncmp(ent->d_name, "event", 5) != 0)
            continue;

        char path[64];
        snprintf(path, sizeof(path), "/dev/input/%s", ent->d_name);
        qInfo() << "  " << path;
    }
    rewinddir(dir);

    qInfo() << "REL/ABS devices:";
    while ((ent = readdir(dir)) != nullptr) {
        if (strncmp(ent->d_name, "event", 5) != 0)
            continue;

        char path[64];
        snprintf(path, sizeof(path), "/dev/input/%s", ent->d_name);

        int probe = ::open(path, O_RDONLY);
        if (probe < 0)
            continue;

        unsigned long evbits[EV_MAX / (8 * sizeof(unsigned long)) + 1] = {};
        if (ioctl(probe, EVIOCGBIT(0, sizeof(evbits)), evbits) < 0) {
            ::close(probe);
            continue;
        }

        bool rel = false, abs = false;
        if (has_bit(evbits, EV_REL)) {
            unsigned long relbits[REL_MAX / (8 * sizeof(unsigned long)) + 1] = {};
            if (ioctl(probe, EVIOCGBIT(EV_REL, sizeof(relbits)), relbits) >= 0)
                rel = has_bit(relbits, REL_X);
        }
        if (has_bit(evbits, EV_ABS)) {
            unsigned long absbits[ABS_MAX / (8 * sizeof(unsigned long)) + 1] = {};
            if (ioctl(probe, EVIOCGBIT(EV_ABS, sizeof(absbits)), absbits) >= 0)
                abs = has_bit(absbits, ABS_X);
        }

        if (rel || abs) {
            qInfo() << "  " << path << "(rel:" << rel << ", abs:" << abs << ")";
            if (fd < 0) {
                qInfo() << "Using input device:" << path;
                fd = probe;
                continue;
            }
        }
        ::close(probe);
    }

    closedir(dir);
    if (fd < 0) {
        qCritical() << "No input device with REL_X or ABS_X found";
        exit(1);
    }
}

void LinuxDragInput::onDrag(std::function<void(int dx, int dy)> cb) {
    callback = std::move(cb);
}

void LinuxDragInput::run() {
    assert(fd >= 0 && "drag input not opened");
    assert(callback && "drag callback not registered");

    while (true) {
        ssize_t n = ::read(fd, buffer, sizeof(buffer));
        assert(n > 0 && "failed to read input events");
        size_t count = (size_t)(n) / sizeof(struct input_event);
        for (size_t i = 0; i < count; ++i) {
            const struct input_event& ev = buffer[i];
            if (ev.type == EV_REL) {
                qDebug() << "REL code=" << ev.code << "value=" << ev.value;
                if (ev.code == REL_X) callback(ev.value, 0);
                else if (ev.code == REL_Y) callback(0, ev.value);
            } else if (ev.type == EV_ABS) {
                qDebug() << "ABS code=" << ev.code << "value=" << ev.value;
                if (ev.code == ABS_X) callback(ev.value, 0);
                else if (ev.code == ABS_Y) callback(0, ev.value);
                else if (ev.code == ABS_MT_POSITION_X) callback(ev.value, 0);
                else if (ev.code == ABS_MT_POSITION_Y) callback(0, ev.value);
            }
        }
    }
}

LinuxDragInput::~LinuxDragInput() {
    if (fd >= 0) {
        ::close(fd);
    }
}
