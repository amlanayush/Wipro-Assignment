#include <iostream>
#include <string>
#include "parser.h"
#include "executor.h"

int main() {
    std::string input;

    while (true) {
        std::cout << "amlan-shell> ";
        std::getline(std::cin, input);  // IMPORTANT: read the whole line

        if (input == "exit") break;
        if (input.empty()) continue;

        Command cmd = Parser::parseInput(input);  //  send full line to parser
        Executor::executeCommand(cmd);            //  pipe/redirect logic now works
    }

    return 0;
}
