#include "client.h"

Client::Client() {
    // setup variables
    buflen_ = 12000;
    buf_ = new char[buflen_+1];
}

Client::~Client() {
}

void Client::run() {
    // connect to the server and run echo program
    create();
    console();
}

void
Client::create() {
}

void
Client::close_socket() {
}

void
Client::console() {
    Debug("Client::console", "Booting up MESSAGEAWESOMENESS!!!");
    string line;
    
    bool inCommand = false;
    state_ = ClientState::Base;
    Request* r;

    // loop to handle user interface
    cout << "% ";
    while (getline(cin,line)) {
        Debug("Client::console", "Got line: "  + line);
        // append a newline
        line += "\n";
        string response;
        bool send = false;
        vector<string>* args;
        int size = 0;
        string command;
        switch (state_) {
            case ClientState::Base:
                Debug("Client::console", "In Base state");
                args = parse_line(line);
                size = args->size();
                if (args->size() > 0) {
                    transform(args->at(0).begin(), args->at(0).end(), args->at(0).begin(), ::tolower); 
                    command = args->at(0);
                    if (command == "send") {
                        if (size == 3) {
                            r = new SendRequest(args->at(1), args->at(2));
                            state(ClientState::Message, "Message");
                        }
                        else { // fail
                            cout << "Invalid number of arguments" << endl;
                            Debug("Client::console", "Invalid number of arguments for send header");
                        }
                    }
                    else if (command == "list") {
                        if (size == 2) {
                            r = new ListRequest(args->at(1));
                            state(ClientState::Listing, "Listing");
                            send = true;
                        }
                        else { // fail
                            cout << "Invalid number of arguments" << endl;
                            Debug("Client::console", "Invalid number of arguments for list command");
                        }
                    }
                    else if (command == "read") {
                        if (size == 3) {
                            r = new ReadRequest(args->at(1), args->at(2));
                            state(ClientState::Reading, "Reading");
                            send = true;
                        }
                        else { // fail
                            cout << "Invalid number of arguments" << endl;
                            Debug("Client::console", "Invalid number of arguments for read command");
                        }
                    }
                    else if (command == "quit") {
                        if (size == 1) {
                            exit(0);
                        }
                    }
                    else {
                        cout << "Command not found" << endl;
                        Debug("Client::console", "Command not found");
                    }
                }
                break;
            case ClientState::Message:
            {
                Debug("Client::console", "In Message state");
                if (SendRequest* sr = static_cast<SendRequest*>(r)) {
                    sr->Message(line);
                    state(ClientState::Message_N, "Message_N");
                }
                else {
                    // invalid state
                    Debug("Client::console", "Invalid state: Message");
                } 
                break;
            }
            case ClientState::Message_N:
            {
                Debug("Client::console", "In Message_N state");
                if (SendRequest* sr = static_cast<SendRequest*>(r)) {
                    Debug("Client::console", "Found send request");
                    if (line == "\n") {
                        send = true;
                        state(ClientState::Sending, "Sending");
                    }
                    else {
                        Debug("Client::console", "Appending to send request");
                        sr->MessageAppend(line);
                        Debug("Client::console", "Contents: " + sr->Message());
                    }
                }
                else {
                    // invalid state
                    Debug("Client::console", "Invalid state: Message_N");
                }
                break;
            }
            case ClientState::Sending:
                Debug("Client::console", "Waiting on response from send");
                break;
            case ClientState::Listing:
                Debug("Client::console", "Waiting on response from list");
                break;
            case ClientState::Reading:
                Debug("Client::console", "Waiting on response from read");
                break;
            default:
                Debug("Client::console", "Invalid state: Unknown");
                break;
        }

        if (send) {
            // send request
            bool success = send_request(r->toString());
            delete r;
            // break if an error occurred
            if (not success)
                break;

            // get a response
            string response = get_response(server_);
            vector<string>* resp_args = parse_line(response);
            int size = resp_args->size();
            if (size > 0) {
//                transform(args->at(0).begin(), args->at(0).end(), args->at(0).begin(), ::tolower); 
                string command = resp_args->at(0);
                switch (state_) {
                    case ClientState::Sending:
                        if (command == "OK") {
                            //continue
                        }
                        else if (command == "error") {
                            cout << "Server returned error: " << response;
                        }
                        else
                            cout << "Server returned invalid response: " << response << endl;
                        break;
                    case ClientState::Listing:
                        if (command == "list") {
                            if (size >= 2) {
                                istringstream buffer(resp_args->at(1));
                                int n;
                                buffer >> n;
                                int nremain = n - size - 2;
                                if (size == 2)
                                    resp_args->push_back("");
                                string response_end = get_response_n(server_, nremain);
                                resp_args->at(2).append(response_end);
                                response.append(response_end);
                                cout << resp_args->at(2);
                            }
                        }
                        else if (command == "error") {
                            cout << "Server returned error: " << response;
                        }
                        else {
                            cout << "Server returned invalid response: " << response;
                        }
                        break;
                    case ClientState::Reading:
                        if (command == "message") {
                            if (size >= 3) {
                                istringstream buffer(resp_args->at(2));
                                int nbytes;
                                buffer >> nbytes;
                                int nremain = nbytes;
                                if (size > 3) {
                                    nremain -= resp_args->at(3).length();
                                }
                                else {
                                    resp_args->push_back("");
                                }
                                string response_end = get_response_nbytes(server_, nremain);
                                resp_args->at(3).append(response_end);
                                response.append(response_end);
                                cout << resp_args->at(1) << endl << resp_args->at(3);
                            }
                        }
                        else if (command == "error") {
                            cout << "Server returned error: " << response;
                        }
                        else {
                            cout << "Server returned invalid response: " << response;
                        }
                        break;
                    default:
                        break;
                        // I don't know yet
                }
            }
            else {
                cout << "Invalid response from server" << endl;
            }
            state(ClientState::Base, "Base");
            // break if an error occurred
            if (not success)
                break;
        }

        if (state_ == ClientState::Base)
            cout << "% ";
    }
    Debug("Client::console", "Closing down MESSAGEAWESOMENESS!!!");
    close_socket();
}

bool
Client::send_request(string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

string
Client::get_response(int connect_to) {
    stringstream ss;
    Debug("Client::get_response", "Receiving response header");
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(connect_to,buf_,buflen_,0);
        ss << nread;
        Debug("Client::get_response", "recv'd " + ss.str() + " bytes");
        if (nread < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                Debug("Client::get_response", "EINTR set - retrying");
                continue;
            }
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
    }
    Debug("Client::get_response", "response header received");
    // a better server would cut off anything after the newline and
    // save it in a cache
    return response;
}

string
Client::get_response_n(int connect_to, int n) {
    stringstream ss;
    Debug("Client::get_response_n", "Receiving rest of response");
    string response = "";
    if (n < 1)
        return response;
    int nremain = n;
    int nfound = 0;
    // read until we get a newline
    while (nfound < nremain) {
        int nread = recv(connect_to,buf_,buflen_,0);
        ss << nread;
        Debug("Client::get_response_n", "recv'd " + ss.str() + " bytes");
        if (nread < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                Debug("Client::get_response_n", "EINTR set - retrying");
                continue;
            }
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
        nfound = count_n(response);
    }
    Debug("Client::get_response_n", "rest of response received");
    // a better server would cut off anything after the newline and
    // save it in a cache
    return response;
}

string
Client::get_response_nbytes(int connect_to, int nbytes) {
    stringstream ss;
    Debug("Client::get_response_nbytes", "Receiving rest of response");
    string response = "";
    if (nbytes < 1) return response;
    int nremain = nbytes;
    // read until we get a newline
    while (nremain > 0) {
        int nread = recv(connect_to,buf_,buflen_,0);
        ss << nread;
        Debug("Client::get_response_nbytes", "recv'd " + ss.str() + " bytes");
        if (nread < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                Debug("Client::get_response_nbytes", "EINTR set - retrying");
                continue;
            }
            else
                // an error occurred, so break out
                Debug("Client::get_response_nbytes", "An error occurred");
                return "";
        } else if (nread == 0) {
            // the socket is closed
            Debug("Client::get_response_nbytes", "Read 0 bytes, socket closed");
            return "";
        }

        // don't read more than we need
        if (nremain < nread) {
            ss << nread - nremain;
            Debug("Server::get_request", ss.str() + " bytes discarded - received more than specified length");
            nread = nremain;
        }

        nremain -= nread;
        response.append(buf_,nread);
    }
    Debug("Client::get_response_nbytes", "rest of response received");
    return response;
}

vector<string>* Client::parse_line(string line) {
    vector<string>* args = new vector<string>();
    char split_char = ' ';

    size_t pos = line.find("\n");
    if (pos == string::npos)
        return args;
    string header = line.substr(0, pos);
    string body = line.substr(pos + 1);

    istringstream iss(header);
    for (string each; getline(iss, each, split_char); args->push_back(each));
    if (!body.empty())
        args->push_back(body);

    return args;
}

int Client::count_n(string arg) {
    int count = 0;
    while (arg.find('\n') != string::npos)
        ++count;
    return count;
}

void Client::state(ClientState newState, string newState_str) {
    Debug("Client::state", "Changing to state " + newState_str);
    state_ = newState;
    return;
}
