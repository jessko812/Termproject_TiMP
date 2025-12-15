/**
 * @file file_handler.h
 * @author Филимонов А.А.
 * @version 1.0
 * @date 15.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл класса FileHandler для работы с файлами
 */

#pragma once
#include <vector>
#include <string>
#include <cstdint>

/**
 * @struct VectorData
 * @brief Структура для хранения векторов, считанных из файла
 */
struct VectorData {
    std::vector<std::vector<int16_t>> vectors; /**< Коллекция векторов (каждый вектор - массив int16_t) */
};

/**
 * @class FileHandler
 * @brief Класс для операций чтения/записи файлов
 */
class FileHandler {
public:
    /**
     * @brief Читает векторы из бинарного файла
     * @param filename Имя входного файла
     * @return Структура VectorData с прочитанными векторами
     * @throw std::runtime_error при ошибках открытия или чтения файла
     */
    static VectorData readVectors(const std::string& filename);
    
    /**
     * @brief Записывает результаты в бинарный файл
     * @param filename Имя выходного файла
     * @param results Вектор результатов для записи
     * @throw std::runtime_error при ошибках создания или записи файла
     */
    static void writeResults(const std::string& filename, const std::vector<int16_t>& results);
    
    /**
     * @brief Читает учетные данные из файла конфигурации
     * @param filename Имя файла конфигурации
     * @return Пара (логин, пароль)
     * @throw std::runtime_error при ошибках открытия или неверном формате файла
     */
    static std::pair<std::string, std::string> readCredentials(const std::string& filename);
};
