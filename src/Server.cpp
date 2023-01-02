
#include "Server.h"

Server::Server(int port)throw (const char*) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw "socket failed";

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(fd, (struct sockaddr*)&server, sizeof(server)) < 0)
        throw "bind failure";
    if (listen(fd, 2) < 0)
        throw "listen failure";

}

void Server::start(ClientHandler& ch)throw(const char*) {
    t = new thread([&ch, this](){
        socklen_t  clientSize = sizeof(client);
        int aClient1, aClient2;

        aClient1 = accept(fd, (struct sockaddr*)&client, &clientSize);
        if (aClient1 < 0)
            throw "accept failure";
        ch.handle(aClient1);
        close(aClient1);

        aClient2 = accept(fd, (struct sockaddr*)&client, &clientSize);
        if (aClient2 < 0)
            throw "accept failure";
        ch.handle(aClient2);
        close(aClient1);

        close(fd);
    });
}

void Server::stop(){
    t->join(); // do not delete this!
}

Server::~Server() {
}

