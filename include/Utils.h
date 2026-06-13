#pragma once

#include <expected>
#include <string>
#include <vector>

namespace utils {
    std::string trim(const std::string& value);
    std::vector<std::string> split(const std::string& value, char delimiter);
    std::string join(const std::vector<std::string>& values, char delimiter);
    std::vector<std::string> splitEscaped(const std::string& value, char delimiter);
    std::string joinEscaped(const std::vector<std::string>& values, char delimiter);
    std::string joinFrom(const std::vector<std::string>& values, std::size_t startIndex, char separator = ' ');

    std::expected<int, std::string> toInt(const std::string& value);
    std::expected<double, std::string> toDouble(const std::string& value);

    bool isValidDate(const std::string& value);
    bool isUpcomingDate(const std::string& value);
    std::string todayText();
    std::string nowText();
}
