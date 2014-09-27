
#include "message-list.h"

MessageList::MessageList() {}

void MessageList::add(string client, Message message) {
    unique_lock<mutex> lck(mutex_);
    messages_[client].push_back(message);
}

Message MessageList::get(string client, int index) {
    unique_lock<mutex> lck(mutex_);
    map<string, vector<Message> >::iterator it = messages_.find(client);
    if (it != messages_.end()) {
        return it->second.at(index);
    }
    throw out_of_range("Requested index out of range in message list");
}

int MessageList::count(string client) {
    unique_lock<mutex> lck(mutex_);
    map<string, vector<Message> >::iterator it = messages_.find(client);
    if (it != messages_.end()) {
        return it->second.size();
    }
    return 0;
}

void MessageList::clear() {
    messages_.clear();
}
