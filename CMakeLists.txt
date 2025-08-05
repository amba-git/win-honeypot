cmake_minimum_required(VERSION 3.10)
project(honeypot)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#Source files path
set(SOURCES
    main.cpp
    _http/src/http_honeypot.cpp
    _http/src/http_log.cpp
    _ssh/src/ssh_honeypot.cpp
    _ssh/src/ssh_log.cpp
)

#executable path
add_executable(honeypot ${SOURCES})

#libraries
target_link_libraries(honeypot ws2_32)
