#include "inet-server.h"

InetServer::InetServer(int port) {
    // setup variables
    port_ = port;
    stringstream ss;
    ss << port;
    portStr_ = ss.str();
}

void
InetServer::create() {
    Debug("InetServer::create", "Creating server on port " + portStr_);
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        Debug("InetServer::create", "Failure creating server");
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        Debug("InetServer::create", "Failure creating server");
        exit(-1);
    }

    // call bind to associate the socket with our local address and
    // port
    if (bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        Debug("InetServer::create", "Failure creating server");
        exit(-1);
    }

      // convert the socket to listen for incoming connections
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        Debug("InetServer::create", "Failure creating server");
        exit(-1);
    }
    Debug("InetServer::create", "Server created on port " + portStr_);
}

void
InetServer::close_socket() {
    Debug("InetServer::close_socket", "Closing socket");
    close(server_);
    Debug("InetServer::close_socket", "Socket closed successfully");
}
