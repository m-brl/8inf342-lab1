#pragma once

#include <memory>
#include <vector>
#include <mutex>
#include <map>
#include <string>

class EnvManager {
private:
    struct Variable {
        std::string value;
        bool exported;
    };
    static std::unique_ptr<EnvManager> _instance;
    static std::mutex _instanceMutex;

    std::map<std::string, Variable> _env;
    mutable std::mutex _envMutex;

    static const std::map<std::string, std::string> _defaultVariable;

    explicit EnvManager() = default;

public:
    static EnvManager& getInstance();

    void initFromRawEnv(char **env);

    [[nodiscard]] std::string getEnv(std::string key) const;
    void setEnv(std::string key, std::string value);

    std::vector<std::string> getKeys() const;
};
