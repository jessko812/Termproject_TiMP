#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "file_handler.h"

class NetworkManager {
private:
    int sockfd;
    std::string address;
    int port;

    void sendData(const void* data, size_t size);
    void receiveData(void* data, size_t size);
    std::string receiveString();
    void sendString(const std::string& str);

public:
    NetworkManager(const std::string& addr, int p);
    ~NetworkManager();
    
    bool connect();
    void disconnect();
    bool authenticate(const std::string& login, const std::string& password);
    std::vector<int16_t> sendVectors(const VectorData& vectors);
};