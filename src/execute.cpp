#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <format>
#include <cstring>
#include <filesystem>

#include "Command.hpp"
#include "EnvManager.hpp"
#include "builtin.hpp"

/**
 * Forks and executes a command
 * @param command The command to execute
 * @param finalPath The path to the executable
 */
void forkAndExecute(Command command, std::string finalPath) {
    pid_t pid = fork();
    char **argv = command.dumpArgv();

    if (pid == 0) {
        execvp(finalPath.c_str(), argv);
        exit(0);
    } else {
        command.setPid(pid);
        Command::addHistory(command);
        int status = 0;
        if (!command.getBackground())
            waitpid(pid, &status, 0);
        else
            std::cout << pid << std::endl;
    }
    for (size_t i = 0; argv[i] != NULL; i++) {
        delete[] argv[i];
    }
    delete[] argv;

}

/**
 * Executes a command, either a built-in or an external command
 * @param command The command to execute
 */
void executeCommand(Command command) {
    if (execute_builtin(command) != -1) {
        return;
    }

    std::filesystem::path executable(command.getArgs()[0]);
    if (command.getArgs()[0][0] == '.') {
        executable = std::filesystem::absolute(command.getArgs()[0]);
    }

    std::string path = EnvManager::getInstance().getEnv("PATH");
    std::istringstream sPath(path);

    while (!sPath.eof()) {
        std::string tmpPath;
        std::getline(sPath, tmpPath, ':');
        std::string finalPath = std::format("{}/{}", tmpPath, executable.string());
        if (std::filesystem::exists(finalPath)) {
            forkAndExecute(command, finalPath);
            return;
        }
    }
    std::cerr << std::format("mysh: {}: command not found", command.getArgs()[0]) << std::endl;
}
