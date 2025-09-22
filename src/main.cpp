#include <iostream>
#include "EnvManager.hpp"
#include <errno.h>
#include <string.h>

int loop();

int main(int ac, char **av, char **env) {
    EnvManager::getInstance().initFromRawEnv(env);
    loop();

    return 0;
}
