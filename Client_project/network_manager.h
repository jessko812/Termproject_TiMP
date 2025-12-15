/**
 * @file network_manager.h
 * @author Филимонов А.А.
 * @version 1.0
 * @date 15.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса NetworkManager для сетевого взаимодействия
 */

#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "file_handler.h"

/**
 * @class NetworkManager
 * @brief Класс для управления сетевым соединением с сервером
 * @details Обеспечивает подключение, аутентификацию и обмен данными с сервером
 */
class NetworkManager {
private:
    int sockfd;                 /**< Дескриптор сетевого сокета */
    std::string address;        /**< Адрес сервера */
    int port;                   /**< Порт сервера */

    /**
     * @brief Отправляет бинарные данные через сокет
     * @param data Указатель на данные
     * @param size Размер данных в байтах
     * @throw std::runtime_error при ошибке отправки
     */
    void sendData(const void* data, size_t size);
    
    /**
     * @brief Принимает бинарные данные через сокет
     * @param data Указатель на буфер для данных
     * @param size Ожидаемый размер данных в байтах
     * @throw std::runtime_error при ошибке приема
     */
    void receiveData(void* data, size_t size);
    
    /**
     * @brief Принимает строку через сокет
     * @return Принятая строка
     * @throw std::runtime_error при ошибке приема
     */
    std::string receiveString();
    
    /**
     * @brief Отправляет строку через сокет
     * @param str Строка для отправки
     * @throw std::runtime_error при ошибке отправки
     */
    void sendString(const std::string& str);

public:
    /**
     * @brief Конструктор NetworkManager
     * @param addr Адрес сервера
     * @param p Порт сервера
     */
    NetworkManager(const std::string& addr, int p);
    
    /**
     * @brief Деструктор NetworkManager
     * @details Автоматически закрывает соединение при уничтожении объекта
     */
    ~NetworkManager();
    
    /**
     * @brief Устанавливает соединение с сервером
     * @return true - соединение установлено
     * @throw std::runtime_error при ошибках создания сокета или подключения
     */
    bool connect();
    
    /**
     * @brief Закрывает соединение с сервером
     */
    void disconnect();
    
    /**
     * @brief Выполняет аутентификацию на сервере
     * @param login Логин пользователя
     * @param password Пароль пользователя
     * @return true - аутентификация успешна, false - аутентификация не удалась
     * @throw std::runtime_error при ошибках сетевого обмена
     */
    bool authenticate(const std::string& login, const std::string& password);
    
    /**
     * @brief Отправляет векторы на сервер и получает результаты вычислений
     * @param vectors Данные векторов для отправки
     * @return Вектор результатов вычислений
     * @throw std::runtime_error при ошибках сетевого обмена
     */
    std::vector<int16_t> sendVectors(const VectorData& vectors);
};
