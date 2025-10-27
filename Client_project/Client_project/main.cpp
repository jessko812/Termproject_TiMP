#include <iostream>
#include <exception>
#include "interface.h"
#include "file_handler.h"
#include "network_manager.h"

int main(int argc, char* argv[]) {
    try {
        Params params;
        
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