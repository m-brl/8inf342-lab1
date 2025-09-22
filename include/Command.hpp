#pragma once

#include <vector>
#include <string>
#include <optional>

class Command {
private:
    std::vector<std::string> _args;

    std::optional<std::string> _stdoutRedirection;
    std::optional<std::string> _stderrRedirection;

public:
    Command() = default;
    Command(std::vector<std::string> args);

    std::vector<std::string> getArgs() const;
    void setArgs(std::vector<std::string> args);
    void addArg(std::string arg);

    char **dumpArgv();
};
