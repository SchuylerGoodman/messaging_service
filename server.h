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

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

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
    void serve();
    void handle(int);
    string get_request(int);
    string get_request(int, int);
    bool send_response(int, string);
    vector<string>* parse_header(string); 

    int server_;
    int buflen_;
    char* buf_;
    map<string, vector<Message> >* messages_;
    string cache_;
};
