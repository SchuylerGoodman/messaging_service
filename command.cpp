
#include "command.h"

string Command::respond(vector<string>* args, MessageList* messages) {
    return "error Invalid command given\n";
}

PutCommand::PutCommand(int client) {
    client_ = client;
}
PutCommand::~PutCommand() {}
string PutCommand::respond(vector<string>* args, MessageList* messages) {
    Debug("PutCommand::respond", "Responding to request");
    ostringstream response;
    int size = args->size();
    if (size < 4 || size > 5) {
        Debug("PutCommand::respond", "Client sent the wrong number of arguments");
        response << "error Invalid request (# of arguments)\n";
    }
    else {
        istringstream buffer(args->at(3));
        int length;
        buffer >> length;
        int nread = 0;
        if (size > 4)
            nread = args->at(4).length();
        else
            args->push_back("");

        int nremain = length - nread;

        string rest_of_request = get_request(client_, nremain);
        args->at(4).append(rest_of_request);

        messages->add(args->at(1), Message(args->at(2), args->at(4), nread + nremain));
        response << "OK\n";
    }
    Debug("PutCommand::respond", "Sending response: " + response.str());
    return response.str();
}

string PutCommand::get_request(int client, int length) {
    char* buf = new char[12001];
    stringstream ss;
    ss << length;
    Debug("PutCommand::get_request", "Receiving request body - length: " + ss.str());
    string request = "";
    int nremain = length;
    // read the rest of the message
    while (nremain > 0) {
        ss.clear();
        ss.str("");
        int nread = recv(client, buf, 1024, 0);
        ss << nread;
        Debug("PutCommand::get_request", "recv'd " + ss.str() + " bytes");
        ss.clear();
        ss.str("");
        if (nread < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                Debug("PutCommand::get_request", "EINTR set - retrying");
                continue;
            }
            else
                // an error occurred, so break out
                Debug("PutCommand::get_request", "An error occurred");
                return "";
        } else if (nread == 0) {
            Debug("PutCommand::get_request", "Read 0 bytes - socket closed");
            return "";
        }

        // don't read more than we need
        if (nremain < nread) {
            ss << nread - nremain;
            Debug("PutCommand::get_request", ss.str() + " bytes discarded - received more than specified length");
            nread = nremain;
        }

        nremain -= nread;
        request.append(buf, nread);
    }
    delete buf;
    Debug("PutCommand::get_request", "Received message request");
    return request;
}

ListCommand::ListCommand() {}
ListCommand::~ListCommand() {}
string ListCommand::respond(vector<string>* args, MessageList* messages) {
    Debug("ListCommand::respond", "Responding to request");
    ostringstream response;
    int size = args->size();
    if (size != 2) {
        Debug("ListCommand::respond", "Client sent the wrong number of arguments for a list request");
        response << "error Invalid request (# of arguments)\n";
    }
    else {
        int count = messages->count(args->at(1));
        if (count == 0) {
            Debug("ListCommand::respond",  "Client requested messages for nonexistent user " + args->at(1));
            response << "error User " << args->at(1) << " does not exist\n";
        }
        else {
            response << "list " << count << "\n";
            for (int i = 0; i < count; ++i) {
                Message m = messages->get(args->at(1), i);
                response << i + 1 << " " << m.Subject() << "\n";
            }
        }
    }
    Debug("ListCommand::respond", "Sending response: " + response.str());
    return response.str();
}

GetCommand::GetCommand() {}
GetCommand::~GetCommand() {}
string GetCommand::respond(vector<string>* args, MessageList* messages) {
    Debug("GetCommand::respond", "Responding to request");
    ostringstream response;
    int size = args->size();
    if (size != 3) {
        Debug("GetCommand::respond", "Client sent the wrong number of arguments for a get request");
        response << "error Invalid request (# of arguments)\n";
    }
    else {
        int count = messages->count(args->at(1));
        if (count == 0) {
            Debug("GetCommand::respond",  "Client requested message for nonexistent user " + args->at(1));
            response << "error User " << args->at(1) << " does not exist\n";
        }
        else {
            istringstream buffer(args->at(2));
            int index;
            buffer >> index;
            if (count <= index - 1) {
                Debug("GetCommand::respond", "Client requested message out of range for user " + args->at(1));
                response << "error Index " << index - 1 << " does not exist for user " << args->at(1) << "\n";
            }
            else {
                Message m = messages->get(args->at(1), index - 1);
                response << "message " <<  m.Subject() << " " << m.Length() << "\n" << m.Content();
            }
        }
    }
    Debug("GetCommand::respond", "Sending response: " + response.str());
    return response.str();
}

ResetCommand::ResetCommand() {}
ResetCommand::~ResetCommand() {}
string ResetCommand::respond(vector<string>* args, MessageList* messages) {
    Debug("ResetCommand::respond", "Responding to request");
    messages->clear();
    string response = "OK\n";
    Debug("ResetCommand::respond", "Sending response: " + response);
    return response;
}
