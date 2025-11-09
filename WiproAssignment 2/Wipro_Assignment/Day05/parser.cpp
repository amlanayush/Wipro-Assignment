#include "parser.h"
#include <sstream>

Command Parser::parseInput(const std::string &input) {
    Command cmd;
    std::stringstream ss(input);
    std::string token;

    bool rightSide = false;  // used for pipe

    while (ss >> token) {

        // 1. Handle PIPE |
        if (token == "|") {
            cmd.isPipe = true;
            rightSide = true;
        }

        // 2. Handle OUTPUT REDIRECTION >
        else if (token == ">") {
            ss >> cmd.outFile;          // get filename
            cmd.outputRedirect = true;
        }

        // 3. Handle INPUT REDIRECTION <
        else if (token == "<") {
            ss >> cmd.inFile;
            cmd.inputRedirect = true;
        }

        // 4. Handle BACKGROUND EXECUTION &
        else if (token == "&") {
            cmd.isBackground = true;
        }

        // 5. Normal command tokens
        else {
            if (!rightSide)
                cmd.args.push_back(token);        // left side
            else
                cmd.pipeRightArgs.push_back(token); // right side
        }
    }

    return cmd;
}
