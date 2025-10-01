#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <thread>
#include <atomic>

#include "Command.hpp"
#include "EnvManager.hpp"

std::atomic<bool> isRunning = true;

/**
 * Detects when background child processes end
 */
void detectEnd() {
    while (isRunning) {
        int status = 0;
        pid_t pid = waitpid(-1, &status, WNOHANG);
        usleep(1000);
    }
}

/**
 * Main loop of the REPL
 * Reads input from stdin, parses it into commands, and executes them
 */
int loop() {
    std::thread endDetector(detectEnd);

    while (isRunning) {
        errno = 0;
        std::string rawInput;
        if (isatty(STDIN_FILENO)) {
            std::cout << EnvManager::getInstance().getEnv("PS1");
        }
        std::getline(std::cin, rawInput);
        if (std::cin.eof()) {
            std::cout << std::endl;
            isRunning = false;
            break;
        }
        Command command(rawInput);
        executeCommand(command);
    }
    endDetector.join();
    return 0;
}
