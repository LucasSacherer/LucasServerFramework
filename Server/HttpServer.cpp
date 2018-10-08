//
// Created by lsacherer on 7/27/18.
//

#include "HttpServer.h"
#include "HttpRequestHandler.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <memory>
#include <thread>

#define BACKLOG 10 //amount of pending connections to hold

bool HttpServer::stop = false;

HttpServer::HttpServer(int port, AppConfig& app): appList(app.getElementList()),
    logger(std::make_unique<Logger>("HttpServer")) {
    HttpServer::port = port;
    HttpServer::errorHandler = app.getErrorHandler();

    //setup the pollrate, make this come from settings
    pollRate.tv_sec = 0;
    pollRate.tv_nsec = 100000000;

    //set the request counter to 0
    request_num_counter = 1;
}

void* HttpServer::get_in_addr (struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void HttpServer::inthand(int signum) {
    stop = true;
}

void HttpServer::run() {
    logger->info({{"message","Starting up server..."}});

    //convert port to c-string
    char* portString = (char*) malloc(12);
    intToCString(port, portString);

    //gets the address info
    struct addrinfo *servinfo = getServerInfo(portString);
    free(portString);

    //bind to the first server that we can in the servinfo linked list
    int sockfd = getBindSocket(servinfo);
    freeaddrinfo(servinfo);
    // done with servinfo

    //start the listening
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    logger->info({{"message","Server is started on port " + std::to_string(port)}});
    logger->info({{"message","Waiting for connections..."}});

    //register the stop signal
    signal(SIGINT, inthand);

    //start polling for connections (only returns if SIGINT (above))
    startPoll(sockfd);

    logger->warn({{"message","Stopping the server..."}});
}

void HttpServer::startPoll(int sockfd) {
    socklen_t sin_size;
    struct sockaddr_storage their_addr;
    int new_fd, count;
    char s[INET6_ADDRSTRLEN];
    fd_set readyToRead;
    while(!stop) {
        FD_ZERO(&readyToRead);
        FD_SET(sockfd, &readyToRead);
        if ((count = pselect(sockfd + 1, &readyToRead, nullptr, nullptr, &pollRate, nullptr)) < 0) {
            logger->warn({{"message","Stopping polling..."}});
            break;
        }
        if (count == 0){
            continue;
        }

        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof(s));

        uint64_t request_num = request_num_counter++;
        logger->info({{"request_num", std::to_string(request_num)}, {"ipaddr",std::string(s)}, {"message","Server got a request!"}});

        //creates a new handler to take this request (pass in the port, the request number, and the appList)
        std::thread newHandler (handleRequest, new_fd, request_num, appList, errorHandler);
        newHandler.detach();
    }
}

void HttpServer::handleRequest(int sockfd, uint64_t request_number, const std::list<Element*>& appList,
        Element* errorHandler){
    //TODO: get this from a parameter
    constexpr size_t bufferSize = 1000 * sizeof(char);
    char* buffer = (char*) malloc(bufferSize);
    memset(buffer, '\0', bufferSize);
    ssize_t bytes_read = read(sockfd, buffer, bufferSize);
    if (bytes_read < 0) {
        perror("read");
        return;
    }
    auto requestString = std::make_unique<std::string>(buffer);
    free(buffer);

    auto handler = std::make_unique<HttpRequestHandler>(request_number, &appList, errorHandler);
    handler->run(*requestString, sockfd);
    close(sockfd);
}

struct addrinfo* HttpServer::getServerInfo(char* portString) {
    int status;
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(nullptr, portString, &hints, &servinfo)) != 0) {
        logger->error({{"message","Failed to start server on port " + std::to_string(port) + ": " + gai_strerror(status)}});
    }

    return servinfo;
}

void HttpServer::intToCString(int x, char* target) {
    memset(target, '\0', sizeof(*target));
    sprintf(target, "%d", x);
}

int HttpServer::getBindSocket(struct addrinfo* servinfo) {
    int yes = 1;
    int sockfd;
    struct addrinfo *p;
    for (p = servinfo; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("server: setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break; //got a bind
    }

    //check that the port binded correctly
    if (p == nullptr) {
        logger->error({{"message","Failed to start server on port " + std::to_string(port) + ": Failed to bind!"}});
        exit(1);
    }

    return sockfd;
}