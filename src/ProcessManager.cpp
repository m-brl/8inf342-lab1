#include "ProcessManager.hpp"

ProcessManager& ProcessManager::getInstance() {
    std::lock_guard lock(_instanceMutex);

    if (_instance.get() == nullptr) {
        _instance = std::unique_ptr<ProcessManager>(new ProcessManager());
    }
    return *_instance.get();
}

std::vector<Process> ProcessManager::getProcesses() const {
    return this->_processes;
}

void ProcessManager::addProcess(Command command, int pid) {
    this->_processes.push_back({pid, command});
}

std::unique_ptr<ProcessManager> ProcessManager::_instance;
std::mutex ProcessManager::_instanceMutex;
