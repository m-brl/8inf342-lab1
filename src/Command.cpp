#include "Command.hpp"

Command::Command(std::vector<std::string> args) :
    _args(args) {}

std::vector<std::string> Command::getArgs() const {
    return this->_args;
}

void Command::setArgs(std::vector<std::string> args) {
    this->_args = args;
}

void Command::addArg(std::string arg) {
    this->_args.push_back(arg);
}
#include <cstring>

char **Command::dumpArgv() {
    char **argv = new char *[this->_args.size() + 1];
    for (size_t i = 0; i < this->_args.size(); i++) {
        argv[i] = new char[this->_args[i].size() + 1];
        strcpy(argv[i], this->_args[i].c_str());
    }
    argv[this->_args.size()] = NULL;
    return argv;
}
