
#pragma once

#include <vector>
#include <sstream>
#include <string>

#include "request.h"

using Util::Debug;
using namespace std;

enum RequestType { PUT, LIST, GET, RESET };

class RequestFactory {
public:
    RequestFactory();
    ~RequestFactory();

    Request* get(string);
    Request* get(string, string);

private:
    map<string, RequestType> strToReqType;
}
