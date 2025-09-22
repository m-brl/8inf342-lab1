#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <format>
#include <map>
#include <format>

#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>

#include "Command.hpp"
#include "EnvManager.hpp"
#include "builtin.hpp"

bool isRunning = true;

void forkAndExecute(Command command, std::string finalPath) {
    pid_t pid = fork();
    char **argv = command.dumpArgv();

    if (pid == 0) {
        execvp(finalPath.c_str(), argv);
        exit(0);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
    }
    for (size_t i = 0; argv[i] != NULL; i++) {
        delete[] argv[i];
    }
    delete[] argv;

}

void executeCommand(std::vector<std::string> args) {
    Command command(args);
    if (execute_builtin(command) != -1) {
        return;
    }
    std::string path = EnvManager::getInstance().getEnv("PATH");
    std::istringstream sPath(path);

    while (!sPath.eof()) {
        std::string tmpPath;
        std::getline(sPath, tmpPath, ':');
        std::string finalPath = std::format("{}/{}", tmpPath, command.getArgs()[0]);
        if (std::filesystem::exists(finalPath)) {
            forkAndExecute(command, finalPath);
            return;
        }
    }
    std::cerr << std::format("mysh: {}: command not found", command.getArgs()[0]) << std::endl;
}

void replaceEnv(std::vector<std::string>& args) {
    auto keys = EnvManager::getInstance().getKeys();

    for (auto& arg: args) {
        auto start = arg.find_first_of("$");
        if (start == arg.npos) {
            continue;
        }
        auto cmp = arg.substr(start + 1);
        for (auto& key: keys) {
            if (strcmp(key.c_str(), cmp.c_str()) == 0) {
                std::cout << "found" << key << std::endl;
            }
        }
    }
}

std::vector<std::string> traitInput(std::string rawInput) {
    std::vector<std::string> args;
    std::istringstream sRawInput(rawInput);

    while (!sRawInput.eof()) {
        std::string tmpArg;
        std::getline(sRawInput, tmpArg, ' ');
        args.push_back(tmpArg);
    }
    replaceEnv(args);
    return args;
}

void traitInputEnhanced(std::string rawInput);

int loop() {
    while (isRunning) {
        std::string rawInput;
        if (isatty(STDIN_FILENO)) {
            std::cout << EnvManager::getInstance().getEnv("PS1");
        }
        std::getline(std::cin, rawInput);
        if (std::cin.eof()) {
            std::cout << std::endl;
            break;
        }
        traitInputEnhanced(rawInput);
        continue;
        auto args = traitInput(rawInput);
        executeCommand(args);
    }
    return 0;
}
