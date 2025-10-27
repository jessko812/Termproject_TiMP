#include "interface.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

bool Interface::parseCommandLine(int argc, char* argv[], Params& params) {
    // Установка значений по умолчанию
    params.server_port = 33333;
    params.config_file = "/.config/vclient.conf";
    
    int opt;
    while ((opt = getopt(argc, argv, "a:p:i:o:c:h")) != -1) {
        switch (opt) {
            case 'a': // Адрес сервера
                params.server_address = optarg;
                break;
            case 'p': // Порт сервера
                params.server_port = std::stoi(optarg);
                break;
            case 'i': // Входной файл
                params.input_file = optarg;
                break;
            case 'o': // Выходной файл
                params.output_file = optarg;
                break;
            case 'c': // Конфигурационный файл
                params.config_file = optarg;
                break;
            case 'h': // Справка
                showHelp();
                return false;
            default:
                std::cerr << "Неизвестный параметр\n";
                showHelp();
                return false;
        }
    }
    
    // Проверка обязательных параметров
    if (params.server_address.empty() || params.input_file.empty() || params.output_file.empty()) {
        std::cerr << "Ошибка: обязательные параметры не указаны\n";
        showHelp();
        return false;
    }
    
    return true;
}

void Interface::showHelp() {
    std::cout << "Использование: client [OPTIONS]\n";
    std::cout << "Обязательные параметры:\n";
    std::cout << "  -a ADDRESS    Адрес сервера\n";
    std::cout << "  -i FILE       Входной файл с векторами\n";
    std::cout << "  -o FILE       Выходной файл для результатов\n";
    std::cout << "Опциональные параметры:\n";
    std::cout << "  -p PORT       Порт сервера (по умолчанию: 33333)\n";
    std::cout << "  -c FILE       Файл конфигурации (по умолчанию: /.config/vclient.conf)\n";
    std::cout << "  -h            Показать эту справку\n";
    std::cout << "\nПример:\n";
    std::cout << "  ./client -a 127.0.0.1 -i vectors.bin -o results.bin -p 33333\n";
}