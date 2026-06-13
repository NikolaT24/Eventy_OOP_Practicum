#include "Command.h"
#include <sstream>

FunctionCommand::FunctionCommand(std::function<void(const std::vector<std::string>&)> action)
    : action(std::move(action)) {}

void FunctionCommand::execute(const std::vector<std::string>& arguments) {
    action(arguments);
}

void CommandRegistry::add(const std::string& name, std::function<void(const std::vector<std::string>&)> action) {
    commands[name] = std::make_unique<FunctionCommand>(std::move(action));
}

bool CommandRegistry::execute(const ParsedCommand& command) const {
    auto iterator = commands.find(command.name);

    if (iterator == commands.end())
        return false;

    iterator->second->execute(command.arguments);
    return true;
}

ParsedCommand parseCommand(const std::string& line) {
    std::stringstream input(line);
    ParsedCommand result;
    input >> result.name;

    std::string argument;
    while (input >> argument)
        result.arguments.push_back(argument);
    return result;
}
