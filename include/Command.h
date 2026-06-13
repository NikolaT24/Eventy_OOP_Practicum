#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct ParsedCommand {
    std::string name;
    std::vector<std::string> arguments;
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const std::vector<std::string>& arguments) = 0;
};

class FunctionCommand final : public Command {
private:
    std::function<void(const std::vector<std::string>&)> action;

public:
    explicit FunctionCommand(std::function<void(const std::vector<std::string>&)> action);
    void execute(const std::vector<std::string>& arguments) override;
};

class CommandRegistry {
private:
    std::unordered_map<std::string, std::unique_ptr<Command>> commands;

public:
    void add(const std::string& name, std::function<void(const std::vector<std::string>&)> action);
    bool execute(const ParsedCommand& command) const;
};

ParsedCommand parseCommand(const std::string& line);
