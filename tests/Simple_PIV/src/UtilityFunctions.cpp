#include "UtilityFunctions.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

std::unordered_map<std::string, std::string> readConfigFile() {
    std::unordered_map<std::string, std::string> configValues;
    std::ifstream configFile("./config.ini");
    std::string line;
    std::cout<<"Test"<<std::endl;
    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            size_t delimiterPos = line.find("=");
            if (delimiterPos != std::string::npos) {
                std::string key = line.substr(0, delimiterPos);
                std::string value = line.substr(delimiterPos + 1);
                configValues[key] = value;
            }
        }
        configFile.close();
    } else {
        std::cerr << "Unable to open config.ini" << std::endl;
    }

    
    return configValues;

}

void PrintCurrentTimeAndMessage(const std::string& message) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << std::put_time(std::localtime(&now), "%H:%M ") << message << std::endl;
}
