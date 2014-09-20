#pragma once

#include "util.h"
#include <string>
#include <sstream>

using Util::Debug;
using namespace std;

class Message {
public:
    Message();
    Message(string, string, int);
    Message(string, string, string);
    ~Message();

    string Subject();
    void Subject(string);
    
    string Content();
    void Content(string);

    int Length();
    void Length(string);

private:
    string _subject;
    string _content;
    int _length;

};
