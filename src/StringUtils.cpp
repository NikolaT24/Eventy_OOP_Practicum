#include "StringUtils.h"
#include <cctype>
#include <sstream>

std::string StringUtils::trim(const std::string& text) {
    int left = 0;
    int right = (int)text.size() - 1;

    while (left < (int)text.size() && std::isspace((unsigned char)text[left])) {
        left++;
    }

    while (right >= left && std::isspace((unsigned char)text[right])) {
        right--;
    }

    return text.substr(left, right - left + 1);
}

std::string StringUtils::escape(const std::string& text) {
    std::string result;

    for (char ch : text) {
        if (ch == '\\') {
            result += "\\\\";
        } else if (ch == '|') {
            result += "\\p";
        } else if (ch == '\n') {
            result += "\\n";
        } else {
            result += ch;
        }
    }

    return result;
}

std::string StringUtils::unescape(const std::string& text) {
    std::string result;

    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == '\\' && i + 1 < (int)text.size()) {
            char next = text[i + 1];

            if (next == '\\') {
                result += '\\';
                i++;
            } else if (next == 'p') {
                result += '|';
                i++;
            } else if (next == 'n') {
                result += '\n';
                i++;
            } else {
                result += text[i];
            }
        } else {
            result += text[i];
        }
    }

    return result;
}

std::vector<std::string> StringUtils::splitEscaped(const std::string& text, char delimiter) {
    std::vector<std::string> parts;
    std::string current;

    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == '\\' && i + 1 < (int)text.size()) {
            current += text[i];
            current += text[i + 1];
            i++;
        } else if (text[i] == delimiter) {
            parts.push_back(unescape(current));
            current.clear();
        } else {
            current += text[i];
        }
    }

    parts.push_back(unescape(current));
    return parts;
}

std::string StringUtils::joinEscaped(const std::vector<std::string>& parts, char delimiter) {
    std::string result;

    for (int i = 0; i < (int)parts.size(); i++) {
        if (i > 0) {
            result += delimiter;
        }

        result += escape(parts[i]);
    }

    return result;
}

std::string StringUtils::joinFrom(const std::vector<std::string>& parts, int startIndex) {
    std::ostringstream result;

    for (int i = startIndex; i < (int)parts.size(); i++) {
        if (i > startIndex) {
            result << " ";
        }

        result << parts[i];
    }

    return result.str();
}

std::optional<int> StringUtils::toInt(const std::string& text) {
    try {
        size_t used = 0;
        int value = std::stoi(text, &used);

        if (used != text.size()) {
            return std::nullopt;
        }

        return value;
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<double> StringUtils::toDouble(const std::string& text) {
    try {
        size_t used = 0;
        double value = std::stod(text, &used);

        if (used != text.size()) {
            return std::nullopt;
        }

        return value;
    } catch (...) {
        return std::nullopt;
    }
}
