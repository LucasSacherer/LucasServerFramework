cmake_minimum_required(VERSION 3.12)
project(firstHttpServer)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost 1.65 COMPONENTS regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(firstHttpServer main.cpp Server/HttpServer.cpp Server/HttpServer.h Server/HttpRequestHandler.cpp Server/HttpRequestHandler.h Server/Element/Element.h Server/Element/Router.cpp Server/Element/Router.h Server/Element/AppConfig.cpp Server/Element/AppConfig.h StandardElements/HelloWorldRouter.cpp StandardElements/HelloWorldRouter.h Server/Log/Logger.cpp Server/Log/Logger.h Server/Element/Element.cpp Server/Element/Filter.cpp Server/Element/Filter.h StandardElements/TimerFilter.cpp StandardElements/TimerFilter.h StandardElements/HttpRequestParser.cpp StandardElements/HttpRequestParser.h StandardElements/SimpleErrorHandler.cpp StandardElements/SimpleErrorHandler.h)
target_link_libraries(firstHttpServer Threads::Threads)
target_link_libraries(firstHttpServer ${Boost_LIBRARIES})