#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct VectorData {
    std::vector<std::vector<int16_t>> vectors;
};

class FileHandler {
public:
    static VectorData readVectors(const std::string& filename);
    static void writeResults(const std::string& filename, const std::vector<int16_t>& results);
    static std::pair<std::string, std::string> readCredentials(const std::string& filename);
};