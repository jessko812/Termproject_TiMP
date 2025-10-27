#pragma once
#include <string>
#include <vector>

class LinuxAuthenticator {
public:
    static std::string computeSHA224(const std::string& data);
    static std::string computeSHA224(const std::string& salt, const std::string& password);
    
private:
    static int createCryptoSocket(const std::string& algorithm);
    static std::vector<uint8_t> computeHash(int crypto_sock, const std::string& data);
};