#include "iumanager.hpp"
#include <mutex>

IUManager& IUManager::instance() {
    static IUManager inst;
    return inst;
}

void IUManager::set_init_wq(int set) {
    iu_fd = set;
}

int IUManager::init_wq() {
    return iu_fd;
}

Work* IUManager::client_front() {
    unique_lock lock(q_mtx);
    return &q.front();
}
