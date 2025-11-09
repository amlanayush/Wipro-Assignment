#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Command {
    std::vector<std::string> args;        // left command
    bool isBackground = false;            // &

    bool isPipe = false;                  // |
    std::vector<std::string> pipeRightArgs;

    bool outputRedirect = false;          // >
    std::string outFile;

    bool inputRedirect = false;           // <
    std::string inFile;
};

class Parser {
public:
    static Command parseInput(const std::string &input);
};

#endif
