#include "EnvManager.hpp"
#include <iostream>

#include <fcntl.h>
#include <signal.h>
#include <sys/signalfd.h>

int loop();

int main(int ac, char **av, char **env) {
    std::cout << "pstree -p " << getpid() << std::endl;
    EnvManager::getInstance().initFromRawEnv(env);
    loop();

    return 0;
}
