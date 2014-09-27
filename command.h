
#pragma once

#include "server.h"
#include "util.h"
#include "message.h"
#include "message-list.h"

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <memory>

using Util::Debug;
using namespace std;

class Command {
    public:
        virtual string respond(vector<string>*, MessageList*);
};

class PutCommand : public Command {
    public:
        PutCommand(int);
        ~PutCommand();

        string respond(vector<string>*, MessageList*);

    private:
        string get_request(int, int);
        int client_;
};

class ListCommand : public Command {
    public:
        ListCommand();
        ~ListCommand();

        string respond(vector<string>*, MessageList*);
};

class GetCommand : public Command {
    public:
        GetCommand();
        ~GetCommand();

        string respond(vector<string>*, MessageList*);
};

class ResetCommand : public Command {
    public:
        ResetCommand();
        ~ResetCommand();

        string respond(vector<string>*, MessageList*);
};
