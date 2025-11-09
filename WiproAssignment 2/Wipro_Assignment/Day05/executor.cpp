#include "executor.h"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <fcntl.h>          // for redirection
#include "jobs.h"           // Job Manager
#include "pipeline.h"       // Pipe handling

void Executor::executeCommand(const Command &cmd) {

    if (cmd.args.empty()) return;

    // Built-in job commands
    if (cmd.args[0] == "jobs") {
        JobManager::listJobs();
        return;
    }
    if (cmd.args[0] == "fg" && cmd.args.size() == 2) {
        int jobId = std::stoi(cmd.args[1].substr(1)); // remove %
        JobManager::bringToForeground(jobId);
        return;
    }
    if (cmd.args[0] == "bg" && cmd.args.size() == 2) {
        int jobId = std::stoi(cmd.args[1].substr(1)); // remove %
        JobManager::moveToBackground(jobId);
        return;
    }

    // PIPE handling (Day 4)
    if (cmd.isPipe) {
        Pipeline::execute(cmd);
        return;
    }

    // Convert args to execvp format
    std::vector<char*> argv;
    for (auto &arg : cmd.args)
        argv.push_back(const_cast<char*>(arg.c_str()));
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0) {
        // Output redirection >
        if (cmd.outputRedirect) {
            int fd = open(cmd.outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                std::cerr << "Failed to open file for output\n";
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        // Input redirection <
        if (cmd.inputRedirect) {
            int fd = open(cmd.inFile.c_str(), O_RDONLY);
            if (fd < 0) {
                std::cerr << "Failed to open file for input\n";
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Execute normal command
        execvp(argv[0], argv.data());
        std::cerr << "Command not found: " << cmd.args[0] << "\n";
        exit(1);
    }

    // Foreground or Background execution
    if (!cmd.isBackground) {
        waitpid(pid, nullptr, 0);
    } else {
        JobManager::addJob(pid, cmd.args[0]);
    }

    // Check completed background jobs
    JobManager::checkCompletedJobs();
}
