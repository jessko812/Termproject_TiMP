#pragma once
#include <string>

struct Params {
    std::string server_address;
    int server_port;
    std::string input_file;
    std::string output_file;
    std::string config_file;
};

class Interface {
public:
    static bool parseCommandLine(int argc, char* argv[], Params& params);
    static void showHelp();
};