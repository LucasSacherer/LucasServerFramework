#include <iostream>
#include "Server/HttpServer.h"
#include "StandardElements/HelloWorldRouter.h"
#include "Server/Log/Logger.h"
#include "StandardElements/TimerFilter.h"
#include "StandardElements/HttpRequestParser.h"
#include "StandardElements/SimpleErrorHandler.h"
#include <memory>

int main() {
    auto logger = std::make_unique<Logger>("Main");
    logger->info({{"message","Hello, World!"}});

    auto app = std::make_unique<AppConfig>();

    auto timerFilter = std::make_unique<TimerFilter>();
    auto requestParser = std::make_unique<HttpRequestParser>();
    auto testRouter = std::make_unique<HelloWorldRouter>();
    app->registerElement(*timerFilter);
    app->registerElement(*requestParser);
    app->registerElement(*testRouter);

    auto errorHandler = std::make_unique<SimpleErrorHandler>();
    app->registerErrorHandler(*errorHandler);

    std::unique_ptr<HttpServer> server(new HttpServer(5000, *app));
    server->run();

    return 0;
}