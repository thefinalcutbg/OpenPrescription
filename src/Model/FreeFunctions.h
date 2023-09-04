#pragma once
#include <string>


namespace FreeFn {

    std::string formatDouble(double price);
    std::string leadZeroes(long long num, int totalLength);
    std::string leadZeroes(const std::string& number, int totalLength);
    std::string getFile(const std::string& filepath);
    std::string removeTrailing(double a);
    std::string getUuid();
    std::string escapeXml(const std::string& data);
};