#include "server.h"

Server::Server() {
    // setup variables
    buflen_ = 12000;
    clients_ = new Buffer();
    messages_ = new MessageList();
    threads_ = new vector<thread>();
    for (int i = 0; i < threadnum_; ++i) {
        threads_->push_back(thread(&Server::run_thread, this));
    }
}

Server::~Server() {
    delete clients_;
    delete messages_;
    delete threads_;
}

void
Server::run() {
    Debug("Server::run", "Attempting to run the server...");
    // create and run the server
    create();
    serve();
    Debug("Server::run", "Server running successfully!");
}

void
Server::create() {
}

void
Server::close_socket() {
}

void Server::run_thread() {
    while (true) {
        int client = clients_->dequeue();
        Debug("Server::run_thread", "Handling client");
        handle(client);
    }
}

void
Server::serve() {
    Debug("Server::serve", "Polling for clients...");
    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

    // accept clients
    while ((client = accept(server_,(struct sockaddr *)&client_addr,&clientlen)) > 0) {
        stringstream ss;
        ss << client;
        Debug("Server::serve", "Client accepted " + ss.str());
        clients_->enqueue(client);
//        handle(client);
    }
    close_socket();
}

void
Server::handle(int client) {
    ostringstream buffer;
    buffer << client;
    Debug("Server::handle", "Handling client requests for client " + buffer.str());

    // loop to handle all requests
    while (1) {
        // get a request
        string request = get_request(client);
        vector<string>* args = parse_header(request);

        string response;
        Command* c;
        int size = args->size();
        if (size > 0) { // If at least the first argument is there (command)
            Debug("Server::handle", "Request: " + args->at(0));
            transform(args->at(0).begin(), args->at(0).end(), args->at(0).begin(), ::tolower); 
            string command = args->at(0);
            if (command == "put") {
                c = new PutCommand(client);
            }
            else if (command == "list") {
                c = new ListCommand();
            }
            else if (command == "get") {
                c = new GetCommand();
            }
            else if (command == "reset") {
                c = new ResetCommand();
            }
            else {
                c = new Command();
            }
            response = c->respond(args, messages_);
            delete c;
        }
        else {
            Debug("Server::handle", "No data sent from client");
            response = "error Invalid request - no command given\n";
        }

        // break if client is done or an error occurred
        if (request.empty()) {
            Debug("Server::handle", "Empty request");
            break;
        }
        
        // send response
        bool success = send_response(client, response);

        // break if an error occurred
        if (not success) {
            Debug("Server::handle", "Problem sending response");
            break;
        }
    }
    Debug("Server::handle", "Done handling client requests");
    close(client);
}

string
Server::get_request(int client) {
    char* buf = new char[buflen_ + 1];
    stringstream ss;
    Debug("Server:get_request", "Receiving request header");
    string request = "";
    // read until we get a newline
    while (request.find("\n") == string::npos) {
        int nread = recv(client,buf,buflen_,0);
        ss << nread;
        Debug("Server::get_request", "recv'd " + ss.str() + " bytes");
        if (nread < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                Debug("Server::get_request", "EINTR set - retrying");
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
        request.append(buf,nread);
    }
    Debug("Server:get_request", "Request header received");
    // a better server would cut off anything after the newline and
    // save it in a cache
    return request;
}

string
Server::get_request(int client, int length) {
    char* buf = new char[buflen_ + 1];
    stringstream ss;
    ss << length;
    Debug("Server::get_request", "Receiving request body - length: " + ss.str());
    string request = "";
    int nremain = length;
    // read the rest of the message
    while (nremain > 0) {
        ss.clear();
        ss.str("");
        int nread = recv(client, buf, buflen_, 0);
        ss << nread;
        Debug("Server::get_request", "recv'd " + ss.str() + " bytes");
        ss.clear();
        ss.str("");
        if (nread < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                Debug("Server::get_request", "EINTR set - retrying");
                continue;
            }
            else
                // an error occurred, so break out
                Debug("Server::get_request", "An error occurred");
                return "";
        } else if (nread == 0) {
            Debug("Server::get_request", "Read 0 bytes - socket closed");
            return "";
        }

        // don't read more than we need
        if (nremain < nread) {
            ss << nread - nremain;
            Debug("Server::get_request", ss.str() + " bytes discarded - received more than specified length");
            nread = nremain;
        }

        nremain -= nread;
        request.append(buf,nread);
    }
    Debug("Server::get_request", "Received message request");
    return request;
}

bool
Server::send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
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

vector<string>*
Server::parse_header(string request) {
    vector<string>* args = new vector<string>();
    char split_char = ' ';

    size_t pos = request.find("\n");
    if (pos == string::npos)
        return args;
    string header = request.substr(0, pos);
    string body = request.substr(pos + 1);

    istringstream iss(header);
    for (string each; getline(iss, each, split_char); args->push_back(each));
    if (!body.empty())
        args->push_back(body);

    return args;
}
