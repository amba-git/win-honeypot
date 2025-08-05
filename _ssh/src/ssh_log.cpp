#include "ssh_log.h"
#include <cstring>

void log_ssh_session(const std::string &ip, const int port, std::string banner){
    
    std::ofstream logFile("ssh_attempt.log",std::ios::app);
    time_t currentTime = time(nullptr);
    char *time_str = ctime(&currentTime);
    time_str[strcspn(time_str, "\n")] = '\0'; 

    logFile <<"Attempt from: "<<ip<<":"<<port<<"\n"
    "At: "<<time_str<<"\n"
    "Client: "<<banner;
    logFile.close();
}