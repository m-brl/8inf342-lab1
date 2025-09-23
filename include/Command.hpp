#pragma once

#include <vector>
#include <string>

class Command {
private:
    std::vector<std::string> _args;
    bool _background;

    void _traitInput(std::string rawInput);

public:
    Command() = default;
    Command(std::string input);
    Command(std::vector<std::string> args);

    std::vector<std::string> getArgs() const;
    void setArgs(std::vector<std::string> args);
    void addArg(std::string arg);

    bool getBackground() const;
    void setBackground(bool background);

    char **dumpArgv();
};
