cmake_minimum_required(VERSION 3.27.4)
project(shell)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include_directories(include)

set(SOURCE_FILE
    src/main.cpp
    src/execute.cpp
    src/loop.cpp
    src/builtin.cpp
    src/Command.cpp
    src/EnvManager.cpp
    src/ProcessManager.cpp
)

add_executable(shell ${SOURCE_FILE})
