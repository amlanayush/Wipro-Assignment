#include "pipeline.h"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <iostream>

void Pipeline::execute(const Command &cmd) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        std::cerr << "Pipe failed\n";
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);

        std::vector<char*> argv;
        for (auto &a : cmd.args)
            argv.push_back(const_cast<char*>(a.c_str()));
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        std::cerr << "Command failed: " << cmd.args[0] << "\n";
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);

        std::vector<char*> argv;
        for (auto &a : cmd.pipeRightArgs)
            argv.push_back(const_cast<char*>(a.c_str()));
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        std::cerr << "Command failed: " << cmd.pipeRightArgs[0] << "\n";
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}
