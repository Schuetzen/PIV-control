#ifndef CUSTOMFUNCTION_H
#define CUSTOMFUNCTION_H

#include <string>
#include <unordered_map>

// Declaration of the readConfigFile function.
std::unordered_map<std::string, std::string> readConfigFile();

// Declaration of the PrintCurrentTimeAndMessage function.
void PrintCurrentTimeAndMessage(const std::string& message);

#endif // CUSTOMFUNCTION_H
