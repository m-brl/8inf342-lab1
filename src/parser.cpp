#include <iostream>
#include <vector>
#include <format>
#include <string>

void sanitizeInput(std::string& rawInput) {
    while (*(rawInput.end() - 1) == ' ') {
        rawInput.erase(rawInput.end() - 1);
    }
}

void traitInputEnhanced(std::string rawInput) {
    sanitizeInput(rawInput);

    std::vector<int> argIndex;
    argIndex.push_back(0);
    int stdoutRedirectionIndex = -1;
    int stderrRedirectionIndex = -1;

    int i = 0;

    bool inhibitNext = false;
    bool inArg = false;
    bool inTextArg = false;

    while (i < rawInput.length()) {
        if (rawInput[i] == '\\')
                inhibitNext = !inhibitNext;
        if (rawInput[i] == '\'')
            inTextArg = !inTextArg;
        if (rawInput[i] == '"' && !inhibitNext)
                inArg = !inArg;
        if (rawInput[i] == ' ' && !inhibitNext) {
            if (!inArg) {
                argIndex.push_back(i + 1);
            }
        }
        if (rawInput[i] == '>' && !inhibitNext) {

        }
        i++;
    }
    if (inArg) {
        std::cerr << "Unmatched '\"'." << std::endl;
        return;
    }

    for (auto index: argIndex) {
        std::cout << index << std::endl;
    }
}
