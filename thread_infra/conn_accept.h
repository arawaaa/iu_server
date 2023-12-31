#pragma once

#include "workbase.h"

class AcceptConns : public Work {
public:
    AcceptConns(int fd);

    work_type type() override;
    int associated_fd() override;
};
