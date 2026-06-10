#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <vector>

struct ParsedCommand {
    std::string name;
    std::vector<std::string> args;
};

class CommandParser {
public:
    static ParsedCommand parse(const std::string& line);
};

#endif
