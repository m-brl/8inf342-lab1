#include "EnvManager.hpp"
#include <iostream>
#include "Command.hpp"

#include <fcntl.h>
#include <sys/signalfd.h>

int loop();

int main(int ac, char **av, char **env) {
    Command::loadHistory();
    EnvManager::getInstance().initFromRawEnv(env);
    loop();
    Command::saveHistory();

    return 0;
}
