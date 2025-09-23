#pragma once

#include "Command.hpp"
#include <memory>
#include <mutex>

struct Process {
    int pid;
    Command command;
};

class ProcessManager {
private:
    static std::unique_ptr<ProcessManager> _instance;
    static std::mutex _instanceMutex;

    std::vector<Process> _processes;

    explicit ProcessManager() = default;

public:
    static ProcessManager& getInstance();

    std::vector<Process> getProcesses() const;
    void addProcess(Command command, int pid);
};
