/**
 * @file interface.h
 * @author Филимонов А.А.
 * @version 1.0
 * @date 15.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для обработки параметров командной строки
 */

#pragma once
#include <string>

/**
 * @struct Params
 * @brief Структура для хранения параметров командной строки программы
 */
struct Params {
    std::string server_address; /**< Адрес сервера для подключения */
    int server_port;            /**< Порт сервера для подключения */
    std::string input_file;     /**< Путь к входному файлу с векторами */
    std::string output_file;    /**< Путь к выходному файлу для результатов */
    std::string config_file;    /**< Путь к файлу конфигурации с учетными данными */
};

/**
 * @class Interface
 * @brief Класс для разбора командной строки и отображения справки
 */
class Interface {
public:
    /**
     * @brief Разбирает аргументы командной строки
     * @param argc Количество аргументов
     * @param argv Массив аргументов
     * @param params Структура для сохранения разобранных параметров
     * @return true - успешный разбор, false - требуется выход (помощь или ошибка)
     */
    static bool parseCommandLine(int argc, char* argv[], Params& params);
    
    /**
     * @brief Отображает справку по использованию программы
     */
    static void showHelp();
};
