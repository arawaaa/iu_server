#include "workbase.h"

Work::Work(int fd) {
    this->fd = fd;
}

work_type Work::type() {
    return work_type::unknown;
}
