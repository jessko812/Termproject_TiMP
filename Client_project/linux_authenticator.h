/**
 * @file linux_authenticator.h
 * @author Филимонов А.А.
 * @version 1.0
 * @date 15.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса LinuxAuthenticator для работы с криптографией через AF_ALG
 */

#pragma once
#include <string>
#include <vector>

/**
 * @class LinuxAuthenticator
 * @brief Класс для вычисления хеш-сумм с использованием Linux Crypto API через сокеты AF_ALG
 * @details Предоставляет статические методы для вычисления SHA-224 хешей для данных и комбинации соли с паролем
 */
class LinuxAuthenticator {
public:
    /**
     * @brief Вычисляет SHA-224 хеш для строки данных
     * @param data Входные данные для хеширования
     * @return SHA-224 хеш в виде шестнадцатеричной строки
     * @throw std::runtime_error при ошибках создания сокета или вычисления хеша
     */
    static std::string computeSHA224(const std::string& data);
    
    /**
     * @brief Вычисляет SHA-224 хеш для комбинации соли и пароля
     * @param salt Соль для хеширования
     * @param password Пароль для хеширования
     * @return SHA-224 хеш в виде шестнадцатеричной строки (соль + пароль)
     * @throw std::runtime_error при ошибках создания сокета или вычисления хеша
     */
    static std::string computeSHA224(const std::string& salt, const std::string& password);
    
private:
    /**
     * @brief Создает криптографический сокет для указанного алгоритма
     * @param algorithm Название алгоритма хеширования (например, "sha224")
     * @return Дескриптор криптографического сокета
     * @throw std::runtime_error при ошибках создания или привязки сокета
     */
    static int createCryptoSocket(const std::string& algorithm);
    
    /**
     * @brief Вычисляет хеш для данных с использованием созданного криптосокета
     * @param crypto_sock Дескриптор криптографического сокета
     * @param data Данные для хеширования
     * @return Вектор байтов с вычисленным хешем
     * @throw std::runtime_error при ошибках передачи данных или получения хеша
     */
    static std::vector<uint8_t> computeHash(int crypto_sock, const std::string& data);
};
