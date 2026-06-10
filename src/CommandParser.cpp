#include "CommandParser.h"
#include <cctype>

ParsedCommand CommandParser::parse(const std::string& line) {
    std::vector<std::string> tokens;
    std::string current;
    bool insideQuotes = false;

    for (int i = 0; i < (int)line.size(); i++) {
        char ch = line[i];

        if (ch == '"') {
            insideQuotes = !insideQuotes;
        } else if (std::isspace((unsigned char)ch) && !insideQuotes) {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += ch;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    ParsedCommand command;

    if (!tokens.empty()) {
        command.name = tokens[0];

        for (int i = 1; i < (int)tokens.size(); i++) {
            command.args.push_back(tokens[i]);
        }
    }

    return command;
}
