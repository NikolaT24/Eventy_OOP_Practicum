#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>
#include <optional>

class StringUtils {
public:
    static std::string trim(const std::string& text);
    static std::vector<std::string> splitEscaped(const std::string& text, char delimiter);
    static std::string joinEscaped(const std::vector<std::string>& parts, char delimiter);
    static std::string escape(const std::string& text);
    static std::string unescape(const std::string& text);
    static std::string joinFrom(const std::vector<std::string>& parts, int startIndex);

    static std::optional<int> toInt(const std::string& text);
    static std::optional<double> toDouble(const std::string& text);
};

#endif
