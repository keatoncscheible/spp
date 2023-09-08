#include <unistd.h>

#include <atomic>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "diagnostics.h"
#include "task.h"

std::atomic<bool> shutting_down(false);

// Signal handler function to handle Ctrl+C
void SignalHandler(int signum) {
    if (signum == SIGINT) {
        std::cout << "\nClosing Application..." << std::endl;
        shutting_down = true;
    }
}

int main(int argc, char* argv[]) {
    signal(SIGINT, SignalHandler);

    task_init();

    while (!shutting_down) {
    }

    task_shutdown();

    return 0;
}
