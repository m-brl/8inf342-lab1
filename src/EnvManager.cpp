#include "EnvManager.hpp"

#include <sstream>
#include <iostream>

EnvManager& EnvManager::getInstance() {
    std::lock_guard lock(_instanceMutex);

    if (_instance.get() == nullptr) {
        _instance = std::unique_ptr<EnvManager>(new EnvManager());
    }
    return *_instance.get();
}

void EnvManager::initFromRawEnv(char **env) {
    // Init env with env passed to main
    for (int i = 0; env[i] != NULL; i++) {
        std::string rawLine = std::string(env[i]);
        std::istringstream sRawLine(rawLine);

        std::string key;
        std::getline(sRawLine, key, '=');

        std::string value;
        std::getline(sRawLine, value, '=');

        this->_env[key].value = value;
        this->_env[key].exported = true;
    }

    // Init env with default variable

    for (const auto& [key, value]: this->_defaultVariable) {
        this->_env[key].value = value;
        this->_env[key].exported = false;
    }
}

std::string EnvManager::getEnv(std::string key) const {
    std::lock_guard lock(this->_envMutex);
    return this->_env.at(key).value;
}

void EnvManager::setEnv(std::string key, std::string value) {
    std::lock_guard lock(this->_envMutex);
    this->_env[key].value = value;
}

std::vector<std::string> EnvManager::getKeys() const {
    std::vector<std::string> keys;

    for (auto& [key, variable]: _env) {
        keys.push_back(key);
    }
    return keys;
}

std::unique_ptr<EnvManager> EnvManager::_instance;
std::mutex EnvManager::_instanceMutex;
const std::map<std::string, std::string> EnvManager::_defaultVariable = {
    {"PS1", "\033[33mGELM0312200300|BORM30090400$\033[0m "}
};
