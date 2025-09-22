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

void executeCommand(Command command) {
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

int loop() {
    while (isRunning) {
        errno = 0;
        std::string rawInput;
        if (isatty(STDIN_FILENO)) {
            std::cout << EnvManager::getInstance().getEnv("PS1");
        }
        std::getline(std::cin, rawInput);
        if (std::cin.eof()) {
            std::cout << std::endl;
            break;
        }
        Command command(rawInput);
        executeCommand(command);
    }
    return 0;
}
