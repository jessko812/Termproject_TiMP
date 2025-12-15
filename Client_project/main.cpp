/**
 * @file main.cpp
 * @author Филимонов А.А.
 * @version 1.0
 * @date 15.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Главный модуль сетевого клиента
 * @details Программа подключается к серверу, выполняет аутентификацию,
 *          отправляет векторы данных и сохраняет результаты вычислений
 */

#include <iostream>
#include <exception>
#include "interface.h"
#include "file_handler.h"
#include "network_manager.h"

/**
 * @brief Точка входа в программу
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return 0 - успешное завершение, 1 - ошибка выполнения
 * @details Основной алгоритм работы программы:
 *          1. Разбор параметров командной строки
 *          2. Чтение учетных данных и векторов из файлов
 *          3. Подключение к серверу и аутентификация
 *          4. Отправка векторов и получение результатов
 *          5. Сохранение результатов в файл
 */
int main(int argc, char* argv[]) {
    try {
        Params params;
        
        // Разбор командной строки
        if (!Interface::parseCommandLine(argc, argv, params)) {
            return 1;
        }
        
        std::cout << "=== Сетевой клиент ===" << std::endl;
        std::cout << "Сервер: " << params.server_address << ":" << params.server_port << std::endl;
        std::cout << "Входной файл: " << params.input_file << std::endl;
        std::cout << "Выходной файл: " << params.output_file << std::endl;
        
        // Чтение учетных данных
        auto [login, password] = FileHandler::readCredentials(params.config_file);
        
        // Чтение векторов
        VectorData vectors = FileHandler::readVectors(params.input_file);
        
        // Сетевое взаимодействие
        NetworkManager netManager(params.server_address, params.server_port);
        netManager.connect();
        
        if (!netManager.authenticate(login, password)) {
            std::cerr << "Ошибка аутентификации" << std::endl;
            return 1;
        }
        
        std::vector<int16_t> results = netManager.sendVectors(vectors);
        netManager.disconnect();
        
        // Сохранение результатов
        FileHandler::writeResults(params.output_file, results);
        
        std::cout << "=============================================" << std::endl;
        std::cout << "Программа успешно завершена!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
