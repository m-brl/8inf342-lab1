#include "Command.hpp"
#include <sstream>
#include <fstream>
#include <cstring>
#include <format>

static bool isBackground(std::string input) {
    for (auto it = input.end() - 1; it != input.begin() - 1; it--) {
        if (*it == ' ')
            continue;
        if (*it == '&')
            return true;
        break;
    }
    return false;
}

void Command::_traitInput(std::string rawInput) {
    if (isBackground(rawInput)) {
        this->_background = true;
        rawInput = rawInput.substr(0, rawInput.find_last_of('&'));
    } else {
        this->_background = false;
    }
    std::istringstream sRawInput(rawInput);

    while (!sRawInput.eof()) {
        std::string tmpArg;
        std::getline(sRawInput, tmpArg, ' ');
        this->_args.push_back(tmpArg);
    }
}

Command::Command() :
    _pid(),
    _background() {}

Command::Command(std::string input) :
    _pid(),
    _rawInput(input),
    _background() {
    this->_traitInput(input);
}

void Command::addHistory(Command command) {
    _history.push_back(std::format("{}\t{}\t{}",
                _historyIndex,
                command._rawInput,
                command._pid));
    _historyIndex++;
}

void Command::loadHistory() {
    std::ifstream file("historique.txt");

    if (!file.is_open())
        return;

    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        if (line.empty())
            continue;
        _history.push_back(line);
        _historyIndex++;
    }
}

void Command::saveHistory() {
    std::ofstream file("historique.txt", std::ios::trunc);

    if (!file.is_open())
        return;

    for (const auto &line: _history) {
        file << line << std::endl;
    }
}

void Command::saveHistory(const std::string& path) {
    std::ofstream file(path, std::ios::trunc);

    if (!file.is_open())
        return;

    for (const auto &line: _history) {
        file << line << std::endl;
    }
}

std::vector<std::string> Command::getHistory() {
    return _history;
}

int Command::getPid() const {
    return this->_pid;
}

void Command::setPid(int pid) {
    this->_pid = pid;
}

std::string Command::getRawInput() const {
    return this->_rawInput;
}

void Command::setRawInput(std::string rawInput) {
    this->_rawInput = rawInput;
    this->_traitInput(rawInput);
}

std::vector<std::string> Command::getArgs() const {
    return this->_args;
}

void Command::setArgs(std::vector<std::string> args) {
    this->_args = args;
}

void Command::addArg(std::string arg) {
    this->_args.push_back(arg);
}

bool Command::getBackground() const {
    return this->_background;
}

void Command::setBackground(bool background) {
    this->_background = background;
}

char **Command::dumpArgv() {
    char **argv = new char *[this->_args.size() + 1];
    for (size_t i = 0; i < this->_args.size(); i++) {
        argv[i] = new char[this->_args[i].size() + 1];
        strcpy(argv[i], this->_args[i].c_str());
    }
    argv[this->_args.size()] = NULL;
    return argv;
}

int Command::_historyIndex = 0;
std::vector<std::string> Command::_history = {};
