 #include "http_log.h"

void log_http_cred(const char* ip, int port, const std::string user_agent, const std::string creds) {
    std::ofstream log_file("http_honeypot.log", std::ios::app);
    if (!log_file) {
        std::cerr << "ERROR, Failed to open honeypot.log\n";
        return;
    }
    
    std::time_t now = std::time(nullptr);
    char* time_str = std::ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';

    log_file << "[" << time_str << "] "
             << ip << ":" << port << "\n"
             << "User-Agent: " << user_agent << "\n"
             << "Credentials: " << creds << "\n\n";
    log_file.close();
    
}