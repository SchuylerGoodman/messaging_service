#pragma once

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "request.h"
#include "util.h"

#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

using Util::Debug;
using namespace std;

enum class ClientState {
    Base,
    Message,
    Message_N,
    Sending,
    Listing,
    Reading
};

class Client {
public:
    Client();
    ~Client();

    void run();

protected:
    virtual void create();
    virtual void close_socket();
    void console();
    bool send_request(string);
    string get_response(int);
    string get_response_n(int, int);
    string get_response_nbytes(int, int);

    int server_;
    int buflen_;
    char* buf_;
    ClientState state_;

private:
    vector<string>* parse_line(string);
    int count_n(string);
    void state(ClientState, string);
};
