#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <format>
#include <map>
#include <format>
#include <thread>

#include <errno.h>
#include <cstring>
#include <filesystem>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/signalfd.h>

#include "Command.hpp"
#include "EnvManager.hpp"
#include "builtin.hpp"

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
