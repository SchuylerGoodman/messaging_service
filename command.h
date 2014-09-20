
#pragma once

#include "server.h"
#include "util.h"
#include "message.h"

#include <string>
#include <vector>
#include <sstream>
#include <map>

using Util::Debug;
using namespace std;

class Command {
    public:
        Command();
        ~Command();

        virtual string respond(vector<string>*, map<string, vector<Message> >*);
};

class PutCommand : public Command {
    public:
        PutCommand(int);
        ~PutCommand();

        string respond(vector<string>*, map<string, vector<Message> >*);

    private:
        string get_request(int, int);
        int client_;
};

class ListCommand : public Command {
    public:
        ListCommand();
        ~ListCommand();

        string respond(vector<string>*, map<string, vector<Message> >*);
};

class GetCommand : public Command {
    public:
        GetCommand();
        ~GetCommand();

        string respond(vector<string>*, map<string, vector<Message> >*);
};

class ResetCommand : public Command {
    public:
        ResetCommand();
        ~ResetCommand();

        string respond(vector<string>*, map<string, vector<Message> >*);
};
