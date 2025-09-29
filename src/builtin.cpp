#include <algorithm>
#include <iostream>
#include <cstring>
#include <format>
#include <string>
#include <map>
#include <fstream>

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

int builtin_launch(Command command) {
    if (command.getArgs().size() < 2) {
        std::cerr << "launch: missing argument" << std::endl;
        return 1;
    }
    std::ifstream file(command.getArgs()[1]);
    if (!file.is_open()) {
        return 1;
    }
    while (!file.eof()) {
        std::string line;
        std::getline(file, line);
        if (line.empty()) {
            continue;
        }
        Command command(line);
        executeCommand(command);
    }
    return 0;
}

int builtin_historique(Command command) {
    for (auto& element: Command::getHistory()) {
        std::cout << element << std::endl;
    }
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
    {"exit", &builtin_exit},
    {"stop", &builtin_exit},
    {"launch", &builtin_launch},
    {"historique", &builtin_historique}
};
