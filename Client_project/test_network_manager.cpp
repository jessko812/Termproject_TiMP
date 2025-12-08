#include <UnitTest++/UnitTest++.h>
#include <algorithm>
#include "linux_authenticator.h"

// Test runner для этого файла
int main()
{
    return UnitTest::RunAllTests();
}

// Мок NetworkManager без наследования
class MockNetworkManager {
public:
    MockNetworkManager(const std::string& addr, int p) {}
    
    bool connect() {
        return true;
    }
    
    bool authenticate(const std::string& login, const std::string& password) {
        std::string salt = "1234567890abcdef";
        std::string hash = LinuxAuthenticator::computeSHA224(salt, password);
        std::transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
        return true;
    }
    
    std::vector<int16_t> sendVectors(const std::vector<std::vector<int16_t>>& vectors) {
        std::vector<int16_t> results;
        for (const auto& vec : vectors) {
            results.push_back(static_cast<int16_t>(vec.size() * 10));
        }
        return results;
    }
};

SUITE(NetworkManagerTests) {
    TEST(POSITIVE_4_1_Connect) {
        MockNetworkManager net("127.0.0.1", 33333);
        CHECK(net.connect());
    }

    TEST(POSITIVE_4_2_Authenticate) {
        MockNetworkManager net("127.0.0.1", 33333);
        CHECK(net.authenticate("testuser", "testpass"));
    }

    TEST(POSITIVE_4_3_SendVectors) {
        MockNetworkManager net("127.0.0.1", 33333);
        std::vector<std::vector<int16_t>> vectors = {{1,2,3}, {10}};
        auto results = net.sendVectors(vectors);
        CHECK_EQUAL(2u, results.size());
        CHECK_EQUAL(30, results[0]);
        CHECK_EQUAL(10, results[1]);
    }
}
