#include <UnitTest++/UnitTest++.h>
#include <fstream>
#include <cstdio>
#include "file_handler.h"

int main()
{
    return UnitTest::RunAllTests();
}

namespace {
    bool removeFile(const std::string& filename) {
        return std::remove(filename.c_str()) == 0;
    }
}

SUITE(FileHandlerTests) {
    TEST(POSITIVE_2_1_ReadVectors) {
        std::string filename = "test_input.bin";
        std::ofstream file(filename, std::ios::binary);
        uint32_t num_vectors = 2;
        file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));
        uint32_t size1 = 3; std::vector<int16_t> vec1 = {1, 2, 3};
        file.write(reinterpret_cast<const char*>(&size1), sizeof(size1));
        file.write(reinterpret_cast<const char*>(vec1.data()), size1 * sizeof(int16_t));
        uint32_t size2 = 2; std::vector<int16_t> vec2 = {10, 20};
        file.write(reinterpret_cast<const char*>(&size2), sizeof(size2));
        file.write(reinterpret_cast<const char*>(vec2.data()), size2 * sizeof(int16_t));
        file.close();

        VectorData data = FileHandler::readVectors(filename);
        CHECK_EQUAL(2u, data.vectors.size());
        CHECK_EQUAL(3u, data.vectors[0].size());
        CHECK_EQUAL(1, data.vectors[0][0]);
        CHECK_EQUAL(2, data.vectors[0][1]);
        CHECK_EQUAL(3, data.vectors[0][2]);
        CHECK_EQUAL(10, data.vectors[1][0]);
        CHECK_EQUAL(20, data.vectors[1][1]);

        removeFile(filename);
    }

    TEST(POSITIVE_2_2_WriteResults) {
        std::string filename = "test_output.bin";
        std::vector<int16_t> results = {42, 123, -456};
        FileHandler::writeResults(filename, results);

        std::ifstream file(filename, std::ios::binary);
        uint32_t num;
        file.read(reinterpret_cast<char*>(&num), sizeof(num));
        CHECK_EQUAL(3u, num);
        std::vector<int16_t> read_results(num);
        file.read(reinterpret_cast<char*>(read_results.data()), num * sizeof(int16_t));
        CHECK_EQUAL(42, read_results[0]);
        CHECK_EQUAL(123, read_results[1]);
        CHECK_EQUAL(-456, read_results[2]);

        removeFile(filename);
    }

    TEST(POSITIVE_2_3_WriteEmptyResults) {
        std::string filename = "test_empty.bin";
        std::vector<int16_t> results;
        FileHandler::writeResults(filename, results);

        std::ifstream file(filename, std::ios::binary);
        uint32_t num;
        file.read(reinterpret_cast<char*>(&num), sizeof(num));
        CHECK_EQUAL(0u, num);

        removeFile(filename);
    }

    TEST(POSITIVE_2_4_ReadCredentials) {
        std::string filename = "test_config.conf";
        std::ofstream file(filename);
        file << "testuser:testpass";
        file.close();

        auto [login, password] = FileHandler::readCredentials(filename);
        CHECK_EQUAL("testuser", login);
        CHECK_EQUAL("testpass", password);

        removeFile(filename);
    }

    TEST(NEGATIVE_2_6_NonExistentFile) {
        CHECK_THROW(FileHandler::readVectors("nonexistent.bin"), std::runtime_error);
    }

    TEST(NEGATIVE_2_7_NoColon) {
        std::string filename = "bad_config.conf";
        std::ofstream file(filename);
        file << "testuser testpass";
        file.close();
        CHECK_THROW(FileHandler::readCredentials(filename), std::runtime_error);
        removeFile(filename);
    }

    TEST(NEGATIVE_2_8_EmptyFile) {
        std::string filename = "empty.conf";
        std::ofstream file(filename);
        file.close();
        CHECK_THROW(FileHandler::readCredentials(filename), std::runtime_error);
        removeFile(filename);
    }

    TEST(NEGATIVE_2_9_OnlyLogin) {
        std::string filename = "only_login.conf";
        std::ofstream file(filename);
        file << "login:";
        file.close();
        auto [login, password] = FileHandler::readCredentials(filename);
        CHECK_EQUAL("login", login);
        CHECK_EQUAL("", password);
        removeFile(filename);
    }

    TEST(NEGATIVE_2_10_OnlyPassword) {
        std::string filename = "only_pass.conf";
        std::ofstream file(filename);
        file << ":password";
        file.close();
        auto [login, password] = FileHandler::readCredentials(filename);
        CHECK_EQUAL("", login);
        CHECK_EQUAL("password", password);
        removeFile(filename);
    }
}
