#pragma once

#include <string>

using namespace std;

enum work_type {
    accept_conns,
    accept_data,
    send_data,
    unknown
};

struct process_data {
    enum RequestType {
        GET,
        PUT
    };

    enum Stage {
        Accept,
        HeaderProcessing,
        Write,
        Read
    };

    Stage stage;
    RequestType reqtype;
    string target;
    string buf;
};
