
#pragma once

#include "util.h"

#include <sstream>
#include <string>

using Util::Debug;
using namespace std;

class Request {
    public:
        Request();
        ~Request();

        virtual string toString() = 0;
};

class SendRequest : public Request {
    public:
        SendRequest(string, string);
        ~SendRequest();

        string toString();
        void Message(string);
        void MessageAppend(string);
        string Message();

    private:
        string user_;
        string subject_;
        string message_;
};

class ListRequest : public Request {
    public:
        ListRequest(string);
        ~ListRequest();

        string toString();

    private:
        string user_;
};

class ReadRequest : public Request {
    public:
        ReadRequest(string, string);
        ~ReadRequest();

        string toString();

    private:
        string user_;
        int index_;
};

