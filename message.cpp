
#include "message.h"

Message::Message() {
    _subject = "";
    _content = "";
}

Message::Message(string subject, string content, string length) {
    _subject = subject;
    _content = content;
    
    istringstream buffer;
    buffer >> _length;
}

Message::Message(string subject, string content, int length) {
    _subject = subject;
    _content = content;
    _length = length;
}

Message::~Message() {
}

string Message::Subject() {
    return _subject;
}

void Message::Subject(string subject) {
    _subject = subject;
}

string Message::Content() {
    return _content;
}

void Message::Content(string content) {
    _content = content;
}

int Message::Length() {
    return _length;
}

void Message::Length(string length) {
    istringstream buffer;
    buffer >> _length;
}
