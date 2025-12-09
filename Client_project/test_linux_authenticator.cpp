#include <UnitTest++/UnitTest++.h>
#include "linux_authenticator.h"

int main()
{
    return UnitTest::RunAllTests();
}

SUITE(LinuxAuthenticatorTests) {
    TEST(POSITIVE_3_1_ComputeSHA224Data) {
        std::string data = "test";
        std::string hash = LinuxAuthenticator::computeSHA224(data);
        CHECK_EQUAL(56u, hash.length());
        for (char c : hash) {
            CHECK((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
        }
    }

    TEST(POSITIVE_3_4_SameInputSameHash) {
        std::string salt = "1234567890abcdef";
        std::string pass = "password";
        std::string hash1 = LinuxAuthenticator::computeSHA224(salt, pass);
        std::string hash2 = LinuxAuthenticator::computeSHA224(salt, pass);
        CHECK_EQUAL(hash1, hash2);
    }

    TEST(POSITIVE_3_5_DifferentPasswords) {
        std::string salt = "1234567890abcdef";
        std::string hash1 = LinuxAuthenticator::computeSHA224(salt, "pass1");
        std::string hash2 = LinuxAuthenticator::computeSHA224(salt, "pass2");
        CHECK(hash1 != hash2);
    }

    TEST(POSITIVE_3_6_DifferentSalts) {
        std::string pass = "password";
        std::string hash1 = LinuxAuthenticator::computeSHA224("salt1", pass);
        std::string hash2 = LinuxAuthenticator::computeSHA224("salt2", pass);
        CHECK(hash1 != hash2);
    }

    TEST(POSITIVE_3_7_HashLength) {
        std::string salt = "1234567890abcdef";
        std::string pass = "password";
        std::string hash = LinuxAuthenticator::computeSHA224(salt, pass);
        CHECK_EQUAL(56u, hash.length());
    }

    TEST(POSITIVE_3_8_NonEmptyHash) {
        std::string data = "testdata";
        std::string hash = LinuxAuthenticator::computeSHA224(data);
        CHECK(!hash.empty());
    }

    TEST(POSITIVE_3_9_SpecialChars) {
        std::string salt = "salt@#";
        std::string pass = "p@ssw#rd!";
        std::string hash = LinuxAuthenticator::computeSHA224(salt, pass);
        CHECK(!hash.empty());
        CHECK_EQUAL(56u, hash.length());
    }

    TEST(POSITIVE_3_2_HexOnly) {
        std::string salt = "0123456789abcdef";
        std::string pass = "pass";
        std::string hash = LinuxAuthenticator::computeSHA224(salt, pass);
        for (char c : hash) {
            CHECK((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
        }
    }
}
