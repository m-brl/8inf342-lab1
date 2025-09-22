#include <algorithm>
#include <iostream>
#include <cstring>
#include <format>
#include <string>
#include <map>

#include "EnvManager.hpp"
#include "builtin.hpp"

extern bool isRunning;

int builtin_cd(Command command) {
    std::string path;
    static std::string lastPath;

    if (command.getArgs().size() == 1 || command.getArgs()[1] == "~") {
        path = EnvManager::getInstance().getEnv("HOME");
    } else if (command.getArgs()[1] == "-") {
        if (lastPath.length() == 0) {
            path = EnvManager::getInstance().getEnv("HOME");
        } else {
            path = lastPath;
        }
    } else {
        path = command.getArgs()[1];
    }
    chdir(path.c_str());
    if (errno != 0) {
        std::cerr << std::format("cd: {}: {}", strerror(errno), path) << std::endl;
        errno = 0;
        return 1;
    }
    return 0;
}

int builtin_exit(Command command) {
    isRunning = false;
    return 0;
}

int execute_builtin(Command command) {
    for (auto& builtin: builtins) {
        if (builtin.first == command.getArgs().at(0)) {
            return builtin.second(command);
        }
    }
    return -1;
}

const std::map<std::string, int(*)(Command command)> builtins = {
    {"cd", &builtin_cd},
    {"exit", &builtin_exit}
};
