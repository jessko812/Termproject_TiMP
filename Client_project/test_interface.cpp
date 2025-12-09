#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <cstring>
#include "interface.h"

int main() {
    return UnitTest::RunAllTests();
}

char* strdup(const char* s) {
    size_t len = strlen(s) + 1;
    char* copy = new char[len];
    std::strcpy(copy, s);
    return copy;
}

SUITE(InterfaceTests) {
    TEST(POSITIVE_HelpFlag) {
        char* arg0 = strdup("./client");
        char* arg1 = strdup("-h");
        char* argv[] = {arg0, arg1, nullptr};
        int argc = 2;
        
        Params params;
        bool result = Interface::parseCommandLine(argc, argv, params);

        CHECK(!result);
        
        delete[] arg0;
        delete[] arg1;
    }

    TEST(NEGATIVE_MissingRequiredParams) {
        char* arg0 = strdup("./client");
        char* arg1 = strdup("-a");
        char* arg2 = strdup("127.0.0.1");
        char* argv[] = {arg0, arg1, arg2, nullptr};
        int argc = 3;
        
        Params params;
        bool result = Interface::parseCommandLine(argc, argv, params);
      
        CHECK(!result);
        
        delete[] arg0;
        delete[] arg1;
        delete[] arg2;
    }

    TEST(NEGATIVE_InvalidOption) {
        char* arg0 = strdup("./client");
        char* arg1 = strdup("-x");
        char* arg2 = strdup("value");
        char* argv[] = {arg0, arg1, arg2, nullptr};
        int argc = 3;
        
        Params params;
        bool result = Interface::parseCommandLine(argc, argv, params);
        
        CHECK(!result);
        
        delete[] arg0;
        delete[] arg1;
        delete[] arg2;
    }
}
