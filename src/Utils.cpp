#include "Utils.h"
#include <algorithm>
#include <chrono>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
    std::string escapeField(const std::string& value, char delimiter) {
        std::string result;
        result.reserve(value.size());

        for (char symbol : value) {
            if (symbol == '\\' || symbol == delimiter || symbol == '\n') {
                result.push_back('\\');
            }

            if (symbol == '\n') {
                result.push_back('n');
            } else {
                result.push_back(symbol);
            }
        }

        return result;
    }
}

std::string utils::trim(const std::string& value) {
    auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char symbol) {
        return std::isspace(symbol) != 0;
    });

    auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char symbol) {
        return std::isspace(symbol) != 0;
    }).base();

    if (first >= last) return "";
    return std::string(first, last);
}

std::vector<std::string> utils::split(const std::string& value, char delimiter) {
    std::vector<std::string> result;
    std::stringstream stream(value);
    std::string current;

    while (std::getline(stream, current, delimiter)) {
        result.push_back(current);
    }

    if (!value.empty() && value.back() == delimiter) {
        result.emplace_back();
    }

    return result;
}

std::string utils::join(const std::vector<std::string>& values, char delimiter) {
    std::ostringstream output;

    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index > 0) output << delimiter;
        output << values[index];
    }

    return output.str();
}

std::vector<std::string> utils::splitEscaped(const std::string& value, char delimiter) {
    std::vector<std::string> result;
    std::string current;
    bool escaped = false;

    for (char symbol : value) {
        if (escaped) {
            current.push_back(symbol == 'n' ? '\n' : symbol);
            escaped = false;
            continue;
        }

        if (symbol == '\\') {
            escaped = true;
            continue;
        }

        if (symbol == delimiter) {
            result.push_back(current);
            current.clear();
            continue;
        }

        current.push_back(symbol);
    }

    if (escaped) current.push_back('\\');
    result.push_back(current);
    return result;
}

std::string utils::joinEscaped(const std::vector<std::string>& values, char delimiter) {
    std::ostringstream output;

    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index > 0) output << delimiter;
        output << escapeField(values[index], delimiter);
    }

    return output.str();
}

std::string utils::joinFrom(const std::vector<std::string>& values, std::size_t startIndex, char separator) {
    if (startIndex >= values.size()) return "";

    std::ostringstream output;

    for (std::size_t index = startIndex; index < values.size(); ++index) {
        if (index > startIndex) output << separator;
        output << values[index];
    }

    return output.str();
}

std::expected<int, std::string> utils::toInt(const std::string& value) {
    try {
        std::size_t consumed = 0;
        int result = std::stoi(value, &consumed);

        if (consumed != value.size()) {
            return std::unexpected("Invalid integer: " + value);
        }

        return result;
    } catch (const std::exception&) {
        return std::unexpected("Invalid integer: " + value);
    }
}

std::expected<double, std::string> utils::toDouble(const std::string& value) {
    try {
        std::size_t consumed = 0;
        double result = std::stod(value, &consumed);

        if (consumed != value.size()) {
            return std::unexpected("Invalid number: " + value);
        }

        return result;
    } catch (const std::exception&) {
        return std::unexpected("Invalid number: " + value);
    }
}

bool utils::isValidDate(const std::string& value) {
    if (value.size() != 10 || value[4] != '-' || value[7] != '-') return false;

    auto year = toInt(value.substr(0, 4));
    auto month = toInt(value.substr(5, 2));
    auto day = toInt(value.substr(8, 2));

    if (!year || !month || !day) return false;
    if (*month < 1 || *month > 12 || *day < 1) return false;

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maximum = daysInMonth[*month - 1];

    bool leap = (*year % 400 == 0) || (*year % 4 == 0 && *year % 100 != 0);
    if (*month == 2 && leap) maximum = 29;

    return *day <= maximum;
}

bool utils::isUpcomingDate(const std::string& value) {
    return isValidDate(value) && value >= todayText();
}

// This date/time formatting code below was generated with AI assistance

std::string utils::todayText() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local{};

#ifdef _WIN32
    localtime_s(&local, &time);
#else
    localtime_r(&time, &local);
#endif

    std::ostringstream output;
    output << std::put_time(&local, "%Y-%m-%d");
    return output.str();
}

std::string utils::nowText() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm local{};

#ifdef _WIN32
    localtime_s(&local, &time);
#else
    localtime_r(&time, &local);
#endif

    std::ostringstream output;
    output << std::put_time(&local, "%Y-%m-%d %H:%M:%S");
    return output.str();
}
