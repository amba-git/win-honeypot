#include "http_honeypot.h"

std::string load_form(){
    std::ifstream html_file("login.html"); //replace to full path
    if(!html_file){
        return "HTTP/1.1 500 Internal Server Error\r\n\r\nLogin page missing";
    }
    std::ostringstream html_content;
    html_content <<html_file.rdbuf();
    std::string html = html_content.str();

    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
            <<"Content-Type: text/html\r\n"
            <<"Content-Length: "<<html.size()<<"\r\n\r\n"
            <<html;
        return response.str();
}

void handle_client(SOCKET sock, std::string client_ip, int client_port){
    char buff[2048] = {"\0"};
    int messg_len = recv(sock, buff, sizeof(buff), 0);
    
    if (messg_len <= 0){
        closesocket(sock);
        return;
    }

    std::string request(buff, messg_len);
    std::string user_agent;
    std::string credentials;

    // User agent extraction
    size_t ua_pos = request.find("User-Agent:");
    if (ua_pos != std::string::npos) {
        size_t ua_end = request.find("\r\n", ua_pos);
        user_agent = request.substr(ua_pos + 12, ua_end - (ua_pos + 12));
    }

    // Handle POST login form
    if (request.find("POST") != std::string::npos) {
        size_t body_pos = request.find("\r\n\r\n");
        if (body_pos != std::string::npos && body_pos + 4 < request.length()) {
            credentials = request.substr(body_pos + 4);
        }
        log_http_cred(client_ip.c_str(), client_port, user_agent, credentials);

        std::string response = "Invalid Username or Password\r\n";
        send(sock, response.c_str(), response.length(), 0);
        closesocket(sock);
        return;
    }

    // Serve login page on GET
    if (request.find("GET") != std::string::npos) {
        std::string login_form = load_form();
        send(sock, login_form.c_str(), login_form.length(), 0);
        closesocket(sock);
        return;
    }

    closesocket(sock);
}

//funtion to start http honeypot service
int http_honeypot_service(const int http_port){
    
    WSADATA wsaData;
    char __client_ip[INET_ADDRSTRLEN];
    int __client_port = 0;
    int result = 0;
    if((result = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0){
        std::cerr<<"WSAStartup failed"<<result<<std::endl;
        return -1;
    }
    const std::string server_ip = "0.0.0.0";
    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_sock == INVALID_SOCKET){
        std::cerr<<"Socket creation failed"<<std::endl;
        return -1;
    }
    
    sockaddr_in server_addr;
    sockaddr_in __client_addr; //for logging client ip address
    
    memset(&server_addr,0,sizeof(server_addr));
    memset(&__client_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET,server_ip.c_str(),&server_addr.sin_addr);
    
    server_addr.sin_port = htons(http_port);

    if(bind(server_sock,(SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR){
        std::cerr<<"Binding socket failed"<<std::endl;
        return -1;
    }
    if(listen(server_sock,SOMAXCONN) == SOCKET_ERROR){ 
            std::cerr<<"Failed to listen for connections"<<std::endl;
            return -1;
    }
    std::cout<<"HTTP Honeypot running on port:"<<http_port<<"\r\n";
    int __client_addr_size = sizeof(__client_addr);

        while(true){
            SOCKET __client_sock = accept(server_sock,(sockaddr*)&__client_addr,&__client_addr_size);
            if(__client_sock == INVALID_SOCKET){
                std::cerr<<"Failed to establish connection"<<std::endl;
                continue;
            }
            inet_ntop(AF_INET,&__client_addr.sin_addr,__client_ip,sizeof(__client_ip));
            __client_port = ntohs(__client_addr.sin_port);
            std::thread(handle_client,__client_sock,std::string(__client_ip),__client_port).detach();
        }
        closesocket(server_sock);
        WSACleanup();

    return 0;
    
}