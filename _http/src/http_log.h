#include <fstream>
#include <ctime>
#include <cstring>
#include <iostream>
#pragma once

//Username, password, IP and user-agent logging
void log_http_cred(const char* ip, int port, const std::string user_agent, const std::string creds);
   