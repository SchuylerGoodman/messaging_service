
#include "request-factory.h"

RequestFactory::RequestFactory() {
    strToReqType = {
        { "put", PUT },
        { "list", LIST },
        { "get", GET },
        { "reset", RESET }
    };
}

RequestFactory::~RequestFactory() {
}

Request*
RequestFactory::get(string header) {
    return get(header, "");
}

Request*
RequestFactory::get(string header, string message) {
    istringstream iss(header);
    vector<string> args;
    char split_char = ' ';

    string each;
    for (each; getline(iss, each, split_char); args.push_back(each));

    RequestType type = strToReqType[args[0]];
    Request* req = null;
    try {
        switch (type) {
            case PUT:
                req = new PutRequest(args, message);
                break;
            case LIST:
                req = new ListRequest(args);
                break;
            case GET:
                req = new GetRequest(args);
                break;
            case RESET:
                req = new ResetRequest(args);
                break;
            default:
                Debug("RequestFactory::get", "Unknown request");
        }
    }
    catch (const out_of_range& e) {
        Debug("RequestFactory::get", "Invalid number of arguments for " + args[0] + " request");
    }
    catch (const invalid_argument& e) {
        Debug("RequestFactory::get", "Invalid arguments given for " + args[0] + " request");
    }
    catch (const exception& e) {
        Debug("RequestFactory::get", "Problem parsing " + args[0] + " request");
    }
    return req;
}
