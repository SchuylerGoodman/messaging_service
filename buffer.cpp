
#include "buffer.h"

Buffer::Buffer() {
    maxBuffer_ = 75;
}

void Buffer::enqueue(int client) {
    unique_lock<mutex> lck(mutex_);
    while (queue_.size() >= maxBuffer_) queueNotFull_.wait(lck);

    queue_.push(client);
    queueNotEmpty_.notify_one();
}

int Buffer::dequeue() {
    unique_lock<mutex> lck(mutex_);
    while (queue_.size() == 0) queueNotEmpty_.wait(lck);

    int client = queue_.front();
    queue_.pop();
    queueNotFull_.notify_one();
    return client;
}
