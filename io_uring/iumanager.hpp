#pragma once

#include "workbase.h"

#include <queue>
#include <utility>
#include <mutex>

#include <liburing.h>
#include <liburing/io_uring.h>

using namespace std;

class IUManager {
    int iu_fd = -1;

    queue<Work> q;
    mutex q_mtx;
    IUManager() = default;
    IUManager(IUManager& oth) = delete;
    IUManager(IUManager&& oth) = delete;
public:
    static IUManager& instance();

    void set_init_wq(int set);
    int init_wq();

    Work* client_front();
    template <typename T, typename... X>
    void add_client(X&&... args);
};

template <typename T, typename... X>
void IUManager::add_client(X&&... args) {
    unique_lock lock(q_mtx);
    q.push(T(forward<X>(args)...));
}
