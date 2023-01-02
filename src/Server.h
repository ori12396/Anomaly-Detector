#ifndef SERVER_H_
#define SERVER_H_

#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <cstring>

#include "CLI.h"
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class SocketIO: public DefaultIO{
    int clientID;

public:
    SocketIO(int clientID) {
        this->clientID = clientID;
    }

    string read() override {
        string text="";
        char c=0;
        ::read(clientID,&c,sizeof(char));
        while(c!='\n'){
            text+=c;
            ::read(clientID,&c,sizeof(char));
        }
        return text;
    }

    void write(string text) override {
        ::send(clientID, text.c_str(), strlen(text.c_str()), 0);
    }

    void write(float f) override {
        //char *s = (char *) &f;
        string str = to_string(f);
        str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
        ::send(clientID, str.c_str(), strlen(str.c_str()), 0);
    }
    void write(int i) override {
        ::send(clientID, to_string(i).c_str(), strlen(to_string(i).c_str()), 0);
    }

    void read(float* f) override {

        string num="";
        char c=0;
        ::read(clientID,&c,sizeof(char));
        while(c!='\n'){
            num+=c;
            ::read(clientID,&c,sizeof(char));
        }
        *f = stof(num, nullptr);
    }
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler {
	public:
    virtual void handle(int clientID) {
        SocketIO dio(clientID);
        CLI cli(&dio);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in client;
    sockaddr_in server;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
