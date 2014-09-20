
#include "request.h"

//++++++++++++++++++++++++++++++++++++REQUEST++++++++++++++++++++++++++++++++++++++
Request::Request() {}
Request::~Request() {}

//++++++++++++++++++++++++++++++++++SENDREQUEST++++++++++++++++++++++++++++++++++++
SendRequest::SendRequest(string user, string subject) {
    user_ = user;
    subject_ = subject;
    message_ = "";
}

SendRequest::~SendRequest() {}

string SendRequest::toString() {
    ostringstream out;
    out << "put " << user_ << " " << subject_ << " " << message_.length() << "\n" << message_;
    Debug("SendRequest::toString", "Returning request: " + out.str());
    return out.str();
}

void SendRequest::Message(string message) {
    message_ = message;
}

void SendRequest::MessageAppend(string message) {
    message_.append(message);
}

string SendRequest::Message() {
    return message_;
}

//++++++++++++++++++++++++++++++++++LISTREQUEST++++++++++++++++++++++++++++++++++++
ListRequest::ListRequest(string user) {
    user_ = user;
}

ListRequest::~ListRequest() {}

string ListRequest::toString() {
    ostringstream out;
    out << "list " << user_ << "\n";
    Debug("ListRequest::toString", "Returning request: " + out.str());
    return out.str();
}

//++++++++++++++++++++++++++++++++++READREQUEST++++++++++++++++++++++++++++++++++++
ReadRequest::ReadRequest(string user, string index) {
    user_ = user;

    istringstream buffer(index);
    buffer >> index_;
}

ReadRequest::~ReadRequest() {}

string ReadRequest::toString() {
    ostringstream out;
    out << "get " << user_ << " " << index_ << "\n";
    Debug("ReadRequest::toString", "Returning request: " + out.str());
    return out.str();
}
