#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>

class DateUtils {
public:
    static bool isValidDate(const std::string& date);
    static std::string nowText();
};

#endif
