#include "ssh_honeypot.h"

void handle_SSH_client(SOCKET client_sock, std::string ip, int port){
    
    std::string banner = "SSH-2.0-OpenSSH_7.4\r\n"; //For port scanners
    send(client_sock,banner.c_str(),banner.length(),0);
    char recv_buff[2048] = {'\0'};
    int recv_mess_len = recv(client_sock,recv_buff,sizeof(recv_buff),0);
    std::string client_banner = (recv_mess_len > 0) ? std::string(recv_buff,recv_mess_len) : "No banner recieved";

    log_ssh_session(ip,port,client_banner);
    closesocket(client_sock);
}

void ssh_honeypot(const int ssh_port){
    WSAData wsaData;
    WSAStartup(MAKEWORD(2,2),&wsaData);

    SOCKET __server_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(__server_sock == INVALID_SOCKET){
        std::fprintf(stderr,"Failed to create a socket: \n");
        return;
    }
    std::string ip_addr = "0.0.0.0"; //change to server ip
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ssh_port);
    inet_pton(AF_INET,ip_addr.c_str(),&server_addr.sin_addr);

    if(bind(__server_sock,(sockaddr*)&server_addr,sizeof(server_addr)) == SOCKET_ERROR){
        std::fprintf(stderr,"Failed to bind the socket\n");
        closesocket(__server_sock);
        return;
    }

    if(listen(__server_sock,SOMAXCONN) == SOCKET_ERROR){
        std::fprintf(stderr,"Failed to listen for incoming connections\n");
        closesocket(__server_sock);
        return;
    }

    std::cout<<"SSH Honeypot running on port: "<<ssh_port<<"\r\n";

    while(true){
        sockaddr_in client_addr;
        memset(&client_addr,0,sizeof(client_addr));
        int client_addr_len = sizeof(client_addr);
        char client_ip[INET_ADDRSTRLEN];
        SOCKET client_sock = accept(__server_sock,(sockaddr*)&client_addr,&client_addr_len);

        if(client_sock == INVALID_SOCKET){
            std::fprintf(stderr,"Failed to accept connection\n");
            continue;
        }
        inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,sizeof(client_ip));
        int client_port = ntohs(client_addr.sin_port);
        std::thread(handle_SSH_client,client_sock,std::string(client_ip),client_port).detach();
    }

}