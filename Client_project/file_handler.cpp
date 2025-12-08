#include "file_handler.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

VectorData FileHandler::readVectors(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    VectorData data;
    uint32_t num_vectors;
    
    file.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));
    if (!file) throw std::runtime_error("Ошибка чтения количества векторов");

    std::cout << "Чтение " << num_vectors << " векторов из файла..." << std::endl;

    for (uint32_t i = 0; i < num_vectors; ++i) {
        uint32_t vector_size;
        file.read(reinterpret_cast<char*>(&vector_size), sizeof(vector_size));
        if (!file) throw std::runtime_error("Ошибка чтения размера вектора");

        std::vector<int16_t> vector(vector_size);
        file.read(reinterpret_cast<char*>(vector.data()), vector_size * sizeof(int16_t));
        if (!file) throw std::runtime_error("Ошибка чтения данных вектора");

        data.vectors.push_back(vector);
        
        std::cout << "  Вектор " << i + 1 << ": размер " << vector_size << std::endl;
    }

    return data;
}

void FileHandler::writeResults(const std::string& filename, const std::vector<int16_t>& results) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось создать файл: " + filename);
    }

    uint32_t num_results = results.size();
    file.write(reinterpret_cast<const char*>(&num_results), sizeof(num_results));
    file.write(reinterpret_cast<const char*>(results.data()), results.size() * sizeof(int16_t));
    
    std::cout << "Записано " << num_results << " результатов в файл: " << filename << std::endl;
}

std::pair<std::string, std::string> FileHandler::readCredentials(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл конфигурации: " + filename);
    }

    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("Пустой файл конфигурации: " + filename);
    }

    size_t pos = line.find(':');
    if (pos == std::string::npos) {
        throw std::runtime_error("Неверный формат файла конфигурации: " + filename);
    }

    std::string login = line.substr(0, pos);
    std::string password = line.substr(pos + 1);
    
    std::cout << "Прочитаны учетные данные: логин=" << login << std::endl;
    
    return {login, password};
}