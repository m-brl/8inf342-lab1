#include "EnvManager.hpp"
#include "Command.hpp"

int loop();

int main(int ac, char **av, char **env) {
    Command::loadHistory();
    EnvManager::getInstance().initFromRawEnv(env);
    loop();
    Command::saveHistory();

    return 0;
}
