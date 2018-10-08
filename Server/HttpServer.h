//
// Created by lsacherer on 7/27/18.
//

#ifndef FIRSTHTTPSERVER_HTTPSERVER_H
#define FIRSTHTTPSERVER_HTTPSERVER_H

#include <time.h>
#include <list>
#include <atomic>
#include "Element/Element.h"
#include "Element/AppConfig.h"
#include "Log/Logger.h"

class HttpServer {
public:
    HttpServer(int port, AppConfig& app);
    void run();

private:
    const std::unique_ptr<Logger> logger;
    static bool stop;

    struct addrinfo* getServerInfo(char* portString);
    int getBindSocket(struct addrinfo* servinfo);

    struct timespec pollRate;
    void startPoll(int sockfd);

    int port;
    void* get_in_addr(struct sockaddr *sa);
    static void inthand(int signum);
    void intToCString(int x, char* target);

    uint64_t request_num_counter;
    //TODO: probably change to passing a reference of the app config to the HttpRequestHandler
    static void handleRequest(int sockfd, uint64_t request_number, const std::list<Element*>& appList, Element* errorHandler);
    const std::list<Element*> appList;
    Element* errorHandler;
};


#endif //FIRSTHTTPSERVER_HTTPSERVER_H
