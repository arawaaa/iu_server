#pragma once

#include "defs.h"

class Work {
protected:
    int fd;
public:
    Work(int fd);

    virtual work_type type();
    virtual int associated_fd() = 0;
};
