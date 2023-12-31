#pragma once

#include "workbase.h"

#include <liburing.h>
#include <linux/io_uring.h>
#include <vector>

using namespace std;

struct TaskInfo {
    bool active;
    Work* w;
};

void start_thread();

class ThreadProcessor {
    vector<TaskInfo> tasks; // Sacrifice throughput for latency, when needed

    io_uring ring;
    io_uring_params io_params;
    bool accept_conns;
    int num;
public:
    ThreadProcessor(bool accept_enable, int globalsock, int num);

    ~ThreadProcessor();

    int exec();
};
