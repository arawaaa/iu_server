#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <vector>
#include <cstring>

#include <boost/program_options.hpp>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "iumanager.hpp"
#include "threadprocessor.h"

using namespace std;

bool use_ipv6;

void parse_options(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    // parse_options(argc, argv);

    addrinfo a1, *a2;
    memset(&a1, 0, sizeof(a1));
    a1.ai_family = use_ipv6 ? AF_INET6 : AF_INET;
    a1.ai_socktype = SOCK_STREAM;
    int mp = getaddrinfo("localhost", "8080", &a1, &a2);
    if (mp != 0) {
        std::cerr << "gai Error\n";
        return 1;
    }

    int globalsock;
    while ((globalsock = socket(a2->ai_family, a2->ai_socktype, a2->ai_protocol)) == -1 && errno == EINTR);
    if (globalsock == -1) {
        std::cerr << "Socket creation failed";
        return 1;
    }

    int res;
    while ((res = bind(globalsock, a2->ai_addr, a2->ai_addrlen)) == -1 && errno == EINTR);
    if (res == -1) {
        return 1;
    }

    while ((res = listen(globalsock, 1024)) == -1 && errno == EINTR);
    if (res == -1) {
        return 1;
    }

    unsigned int tnums = thread::hardware_concurrency();
    vector<thread> threads;
    vector<ThreadProcessor> eus;
    threads.reserve(tnums); eus.reserve(tnums);

    for (unsigned int i = 0; i < tnums; i++) {
        eus.emplace_back(i == 0, globalsock, i);
        threads.emplace_back(&ThreadProcessor::exec, &eus[i]);
    }


    for (unsigned int i = 0; i < tnums; i++) {
        threads[i].join();
    }

    return 0;
}

void print_usage(int argc, char* argv[]) {
    char* exec_name = (char*)"<server-exec>";
    if (argv[0])
        exec_name = argv[0];
    std::cout << exec_name << " [options] port\n";
}
