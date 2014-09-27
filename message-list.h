
#pragma once

#include "util.h"
#include "message.h"

#include <vector>
#include <mutex>
#include <condition_variable>
#include <map>

using Util::Debug;
using namespace std;

class MessageList {
    public:
        MessageList();

        void add(string, Message);
        Message get(string, int);
        int count(string);
        void clear();

    private:
        mutex mutex_;
        map<string, vector<Message> > messages_;
};
