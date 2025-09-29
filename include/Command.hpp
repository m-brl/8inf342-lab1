#pragma once

#include <vector>
#include <string>

class Command {
private:
    int _pid;
    std::string _rawInput;
    std::vector<std::string> _args;
    bool _background;

    void _traitInput(std::string rawInput);

    static int _historyIndex;
    static std::vector<std::string> _history;

public:
    Command() = default;
    Command(std::string input);

    static void addHistory(Command command);
    static void loadHistory();
    static void saveHistory();
    static std::vector<std::string> getHistory();

    int getPid() const;
    void setPid(int pid);

    std::string getRawInput() const;
    void setRawInput(std::string rawInput);

    std::vector<std::string> getArgs() const;
    void setArgs(std::vector<std::string> args);
    void addArg(std::string arg);

    bool getBackground() const;
    void setBackground(bool background);

    char **dumpArgv();
};

void executeCommand(Command command);
