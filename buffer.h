
#pragma once

#include "util.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using Util::Debug;
using namespace std;

class Buffer {
public:
    Buffer();

    void enqueue(int);
    int dequeue();

private:
    queue<int> queue_;
    mutex mutex_;
    condition_variable queueNotEmpty_;
    condition_variable queueNotFull_;

    int maxBuffer_;

};
