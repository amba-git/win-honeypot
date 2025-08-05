#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <thread>
#include <string>
#include <sstream>
#include "http_log.h"
#pragma comment(lib,"ws2_32.lib")
#pragma once

//function for loading login form
std::string load_form();

// handling individual client
void handle_client(SOCKET sock, std::string client_ip, int client_port);

//funtion to start http service
int http_honeypot_service(const int http_port);