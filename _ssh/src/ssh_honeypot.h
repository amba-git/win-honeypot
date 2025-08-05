#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <thread>
#include "ssh_log.h"
#pragma comment(lib,"ws2_32.lib")
#pragma once

//handle individual ssh session
void handle_SSH_client(SOCKET client_sock, std::string ip, int port);

//main honeypot logic
void ssh_honeypot(const int ssh_port);
