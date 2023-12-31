#include "conn_accept.h"

AcceptConns::AcceptConns(int fd)
    : Work(fd)
{

}

work_type AcceptConns::type() {
    return work_type::accept_conns;
}

int AcceptConns::associated_fd() {
    return fd;
}
