#include "_http\src\http_honeypot.h"
#include "_http\src\http_log.h"
#include "_ssh\src\ssh_honeypot.h"
#include "_ssh\src\ssh_log.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

int main(void){
    std::ifstream config_file("config\\config.json");
    if(!config_file){
        std::fprintf(stderr,"Failed to open config.json\n");
        exit(0);
    }

    nlohmann::json config;

    try{
        config_file >> config;
    }catch(std::exception &e){
        std::fprintf(stderr,"Error parsing the config.json file",e.what());
        return 1;
    }

    std::vector<std::thread> threads;

    if(config.contains("enable_http") && config["enable_http"].is_boolean() && config["enable_http"] == true){
        int http_port = 8080; //only if port is not specified
        if(config.contains("http_port") && config["http_port"].is_number_integer()){
            http_port = config["http_port"];
        }
        threads.emplace_back(http_honeypot_service,http_port);
    }

    if(config.contains("enable_ssh") && config["enable_ssh"].is_boolean() && config["enable_ssh"] == true){
        int ssh_port = 2222; //only if not specified
        if(config.contains("ssh_port") && config["ssh_port"].is_number_integer()){
            ssh_port = config["ssh_port"];
        }
        threads.emplace_back(ssh_honeypot,ssh_port);
        
    }

    if(threads.empty()){
        fprintf(stderr,"Error: No services enabled in config.json.\n");
        return 1;
    }

    for(auto &thread: threads){
        if(thread.joinable())
            thread.join();
    }

    return 0;
    
}