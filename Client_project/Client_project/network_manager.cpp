#include "network_manager.h"
#include "linux_authenticator.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <algorithm> 
#include <cctype>

NetworkManager::NetworkManager(const std::string& addr, int p) 
    : sockfd(-1), address(addr), port(p) {}

NetworkManager::~NetworkManager() {
    disconnect();
}

bool NetworkManager::connect() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        throw std::runtime_error("Ошибка создания сокета");
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, address.c_str(), &serv_addr.sin_addr) <= 0) {
        throw std::runtime_error("Неверный адрес: " + address);
    }

    std::cout << "Подключение к " << address << ":" << port << "..." << std::endl;
    if (::connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Ошибка подключения к " + address + ":" + std::to_string(port));
    }

    std::cout << "Подключение установлено успешно" << std::endl;
    return true;
}

void NetworkManager::disconnect() {
    if (sockfd != -1) {
        close(sockfd);
        sockfd = -1;
        std::cout << "Соединение закрыто" << std::endl;
    }
}

void NetworkManager::sendData(const void* data, size_t size) {
    ssize_t bytes_sent = send(sockfd, data, size, 0);
    if (bytes_sent != static_cast<ssize_t>(size)) {
        throw std::runtime_error("Ошибка отправки данных");
    }
}

void NetworkManager::receiveData(void* data, size_t size) {
    ssize_t bytes_received = recv(sockfd, data, size, 0);
    if (bytes_received != static_cast<ssize_t>(size)) {
        throw std::runtime_error("Ошибка приема данных");
    }
}

std::string NetworkManager::receiveString() {
    char buffer[1024];
    ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        throw std::runtime_error("Ошибка приема строки");
    }
    buffer[bytes_received] = '\0';
    return std::string(buffer);
}

void NetworkManager::sendString(const std::string& str) {
    sendData(str.c_str(), str.length());
}

bool NetworkManager::authenticate(const std::string& login, const std::string& password) {
    try {
        //Отправка логина
        sendString(login);
        std::cout << "Отправлен логин: " << login << std::endl;
        
        //Получение соли от сервера или ERR
        std::string salt_response = receiveString();
        std::cout << "Получена соль от сервера " <<std::endl;
        
        if (salt_response == "ERR") {
            std::cerr << "Сервер вернул ошибку идентификации" << std::endl;
            return false;
        }
        
        //Вычисление хеша SHA-224(SALT16 || PASSWORD)
        std::string hash = LinuxAuthenticator::computeSHA224(salt_response, password);
        
        // ПРЕОБРАЗОВАНИЕ В ВЕРХНИЙ РЕГИСТР
        std::transform(hash.begin(), hash.end(), hash.begin(), 
                      [](unsigned char c) { return std::toupper(c); });
        
        sendString(hash);
        std::cout << "Отправлен хеш SHA-224 " << std::endl;
        
        // Получение результата аутентификации
        std::string auth_response = receiveString();
        std::cout << "Результат аутентификации: " << auth_response << std::endl;
        
        return auth_response == "OK";
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка аутентификации: " + std::string(e.what()));
    }
}

std::vector<int16_t> NetworkManager::sendVectors(const VectorData& vectors) {
    try {
        std::vector<int16_t> results;
        
        // Отправка количества векторов (uint32_t)
        uint32_t num_vectors = vectors.vectors.size();
        sendData(&num_vectors, sizeof(num_vectors));
        std::cout << "Отправлено векторов: " << num_vectors << std::endl;
        
        // Отправка каждого вектора и получение результатов
        for (size_t i = 0; i < vectors.vectors.size(); ++i) {
            const auto& vector = vectors.vectors[i];
            
            // Отправка размера вектора (uint32_t)
            uint32_t vector_size = vector.size();
            sendData(&vector_size, sizeof(vector_size));
            
            // Отправка значений вектора (int16_t)
            sendData(vector.data(), vector_size * sizeof(int16_t));
            
            // Получение результата вычислений (int16_t)
            int16_t result;
            receiveData(&result, sizeof(result));
            results.push_back(result);
            
            std::cout << "  Вектор " << i + 1 << " (размер " << vector_size << ") -> результат: " << result << std::endl;
        }
        
        std::cout << "Все векторы успешно обработаны" << std::endl;
        return results;
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Ошибка отправки векторов: " + std::string(e.what()));
    }
}