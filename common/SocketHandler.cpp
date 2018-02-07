#include "SocketHandler.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../client/Client.hpp"

using std::cout;
using std::endl;

namespace socketLibrary{
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}
namespace chat{

SocketHandler::SocketHandler(){
    cout << "SocketHandler constructor" << endl;
    sfd = socketLibrary::socket(AF_INET, socketLibrary::SOCK_STREAM, 0);

    if(sfd == ERROR_SFD){
        error("Error when creating socket");
    }
}

SocketHandler::SocketHandler(int sfd){
    cout << "SocketHandler constructor -> sfd" << endl;

    this->sfd = sfd;

    if(this->sfd == ERROR_SFD){
        cout << "No file descriptor here+ here" << endl;
        error("Error when creating socket");
    }
}

SocketHandler::~SocketHandler(){
    cout << "SocketHandler destructor" << endl;
    if(sfd != ERROR_SFD){
        if(socketLibrary::close(this->sfd)){
            //logger sfd err closed
        }
        sfd = ERROR_SFD;
    }
}

void SocketHandler::error(const char* msg){
    perror(msg);
    exit(1);
}

void SocketHandler::bind(int port){
    using namespace socketLibrary;
    cout << "bind" << endl;
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(socketLibrary::bind(this->sfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0){
        error("Error to bind socket");
     }
}

void SocketHandler::printSfd(){
    cout << this->sfd << endl;
}

void SocketHandler::listen(){
    cout << "listen" << endl;
    if(socketLibrary::listen(this->sfd,this->backlog) < 0){
        error("Error in listen method");
    }
}

client::Client SocketHandler::accept(){
    using namespace socketLibrary;
    cout << "accept" << endl;
    char str[INET_ADDRSTRLEN];
    struct sockaddr_in addr;
    int clientSfd = socketLibrary::accept(this->sfd,(struct socketLibrary::sockaddr*) &addr, NULL);
    inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
    client::Client client(clientSfd,str);
    //client::Client *client = new client::Client((socketLibrary::accept(this->sfd,(struct socketLibrary::sockaddr*) addr, NULL)));
    return client;
}

int SocketHandler::connect(const string ip,int port){
    using namespace socketLibrary;
    struct socketLibrary::sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(struct socketLibrary::sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = port;
    if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) != 1)
    {
        cout << "Error to set ip into serv_addr" << endl;
        return -1;
    }

    return socketLibrary::connect(this->sfd, (struct socketLibrary::sockaddr*)&serv_addr,sizeof(serv_addr));

}

void SocketHandler::write(const char* buff){
    socketLibrary::write(this->sfd, buff, strlen(buff));
}
//check it again
/*int SocketHandler::read(char* recvBuff){
    return socketLibrary::read(this->sfd, recvBuff, strlen(recvBuff));
}*/

void SocketHandler::send(SocketHandler sktAccept, const char *str){
    if(socketLibrary::send(sktAccept.sfd, str, strlen(str), 0) < 0){
        error("Cannot send data");
    }
}

void SocketHandler::read(SocketHandler sktAccept, char *buff, int buffSize){
    if(socketLibrary::read(sktAccept.sfd, buff, buffSize) < 0){
        error("Cannot read");
    }
}
}
