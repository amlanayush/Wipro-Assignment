#include "jobs.h"
#include <iostream>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

std::vector<Job> JobManager::jobs;
int JobManager::nextJobId = 1;

void JobManager::addJob(pid_t pid, const std::string &cmd) {
    Job job;
    job.jobId = nextJobId++;
    job.pid = pid;
    job.command = cmd;
    job.running = true;

    jobs.push_back(job);

    std::cout << "[" << job.jobId << "] " << pid << " started in background\n";
}

void JobManager::listJobs() {
    for (auto &job : jobs) {
        std::cout << "[" << job.jobId << "] "
                  << (job.running ? "Running " : "Stopped ")
                  << "PID: " << job.pid
                  << "  Command: " << job.command << "\n";
    }
}

void JobManager::bringToForeground(int jobId) {
    for (auto &job : jobs) {
        if (job.jobId == jobId) {
            std::cout << "Bringing job to foreground: " << job.command << "\n";
            kill(job.pid, SIGCONT);
            waitpid(job.pid, nullptr, 0);
            job.running = false;
            return;
        }
    }
    std::cout << "fg: job not found\n";
}

void JobManager::moveToBackground(int jobId) {
    for (auto &job : jobs) {
        if (job.jobId == jobId) {
            std::cout << "Resuming job in background: " << job.command << "\n";
            kill(job.pid, SIGCONT);
            job.running = true;
            return;
        }
    }
    std::cout << "bg: job not found\n";
}

void JobManager::checkCompletedJobs() {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (auto &job : jobs) {
            if (job.pid == pid) {
                std::cout << "\n[Job Completed] PID " << pid
                          << " (" << job.command << ")\n";
                job.running = false;
            }
        }
    }
}
