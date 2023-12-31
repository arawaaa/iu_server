#include <iostream>

#include <liburing.h>
#include <linux/io_uring.h>
#include <linux/time_types.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include "threadprocessor.h"
#include "iumanager.hpp"
#include "defs.h"

ThreadProcessor::ThreadProcessor(bool accept_enable, int globalsock, int num)
    : accept_conns(accept_enable)
{
    this->num = num;
    IUManager& inst = IUManager::instance();

    io_params = {0};
    if (!accept_enable) {
        io_params.flags |= IORING_SETUP_ATTACH_WQ;
        io_params.wq_fd = inst.init_wq();
    }

    io_uring_queue_init_params(512, &ring, &io_params);
    std::cout << "Run\n";

    if (accept_enable) {
        inst.set_init_wq(ring.ring_fd);


        process_data* pd = new process_data;
        pd->stage = process_data::Accept;
        io_uring_sqe* sq = io_uring_get_sqe(&ring);
        io_uring_sqe_set_data(sq, pd);
        io_uring_prep_multishot_accept(sq, globalsock, NULL, NULL, SOCK_CLOEXEC);
        io_uring_submit(&ring);
    }
}

ThreadProcessor::~ThreadProcessor() {
    io_uring_queue_exit(&ring);
}

int ThreadProcessor::exec() {
    __kernel_timespec tspec = {0, 500};
    io_uring_cqe *cqe;
    sigset_t sigset;
    sigemptyset(&sigset);
    while (1) {
        std::cout << "Wakeup\n";
        io_uring_wait_cqe(&ring, &cqe);

        std::cout << num << std::endl;
        io_uring_cqe_seen(&ring, cqe);
        // io_uring_wait_cqes(ring, &cqe, 32, &tspec, &sigset);
        // process_data* pd = (process_data*)cqe->user_data;
        // process_data* newpd;
        //
        // switch (pd->stage) {
        //     case process_data::Accept:
        //         newpd = new process_data;
        //         newpd->stage = process_data::HeaderProcessing;
        //         break;
        //     case process_data::HeaderProcessing:
        //     case process_data::Write:
        //     case process_data::Read:
        //
        // }

    }
    return 0;
}
