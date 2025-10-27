#include "linux_authenticator.h"
#include <linux/if_alg.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

int LinuxAuthenticator::createCryptoSocket(const std::string& algorithm) {
    // Создание криптосокета
    int sock = socket(AF_ALG, SOCK_SEQPACKET, 0);
    if (sock == -1) {
        throw std::runtime_error("Ошибка создания криптосокета");
    }

    // Заполнение структуры для хеш-функции
    struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type = "hash",
    };
    
    // копирование имени алгоритма
    std::strncpy(reinterpret_cast<char*>(sa.salg_name), algorithm.c_str(), sizeof(sa.salg_name) - 1);
    sa.salg_name[sizeof(sa.salg_name) - 1] = '\0';

    // Привязка сокета к алгоритму
    if (bind(sock, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        close(sock);
        throw std::runtime_error("Ошибка привязки криптосокета к алгоритму: " + algorithm);
    }

    return sock;
}

std::vector<uint8_t> LinuxAuthenticator::computeHash(int crypto_sock, const std::string& data) {
    // Принятие соединения для криптоопераций
    int op_sock = accept(crypto_sock, nullptr, nullptr);
    if (op_sock == -1) {
        throw std::runtime_error("Ошибка принятия криптосоединения");
    }

    try {
        // Передача данных для вычисления хеша
        ssize_t sent = send(op_sock, data.c_str(), data.length(), 0);
        if (sent != static_cast<ssize_t>(data.length())) {
            close(op_sock);
            throw std::runtime_error("Ошибка передачи данных для хеширования");
        }

        // Получение результата 
        std::vector<uint8_t> hash(28); // SHA-224 produces 28 bytes
        ssize_t received = recv(op_sock, hash.data(), hash.size(), 0);
        if (received != static_cast<ssize_t>(hash.size())) {
            close(op_sock);
            throw std::runtime_error("Ошибка получения хеша");
        }

        close(op_sock);
        return hash;

    } catch (...) {
        close(op_sock);
        throw;
    }
}

std::string LinuxAuthenticator::computeSHA224(const std::string& data) {
    int crypto_sock = -1;
    try {
        // Подготовка: создание криптосокета
        crypto_sock = createCryptoSocket("sha224");
        
        // Вычисление: передача данных и получение хеша
        std::vector<uint8_t> hash = computeHash(crypto_sock, data);
        
        // Преобразование в hex-строку
        std::stringstream ss;
        for (uint8_t byte : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
        }
        
        close(crypto_sock);
        return ss.str();

    } catch (...) {
        if (crypto_sock != -1) {
            close(crypto_sock);
        }
        throw;
    }
}

std::string LinuxAuthenticator::computeSHA224(const std::string& salt, const std::string& password) {
    // Конкатенация соли и пароля 
    std::string data = salt + password;
    std::cout << "Вычисление SHA-224 для соли + пароля..." << std::endl;
    std::string hash = computeSHA224(data);
    
    return hash;
}