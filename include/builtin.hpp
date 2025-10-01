#pragma once

#include <string>
#include <map>

#include "Command.hpp"

int execute_builtin(Command command);
extern const std::map<std::string, int(*)(Command command)> builtins;
