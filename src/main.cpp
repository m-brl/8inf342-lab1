#include "EnvManager.hpp"
#include <signal.h>

int loop();

void sighandler(int signum) {
}

int main(int ac, char **av, char **env) {
    signal(SIGINT, &sighandler);
    EnvManager::getInstance().initFromRawEnv(env);
    loop();

    return 0;
}
