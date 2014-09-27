#pragma once

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "util.h"
#include "message.h"
#include "command.h"
#include "buffer.h"
#include "message-list.h"

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>

using Util::Debug;
using namespace std;

class Server {
public:
    Server();
    ~Server();

    void run();
    
protected:
    virtual void create();
    virtual void close_socket();
    void run_thread();
    void serve();
    void handle(int);
    string get_request(int);
    string get_request(int, int);
    bool send_response(int, string);
    vector<string>* parse_header(string); 

    int server_;
    int buflen_;
    int threadnum_ = 10;
    Buffer* clients_;
    MessageList* messages_;
    vector<thread>* threads_;
    string cache_;
};
