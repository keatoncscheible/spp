#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "task.h"

int main(int argc, char* argv[]) {
    int opt;
    bool diagnostics_flag = false;

    while ((opt = getopt(argc, argv, "d")) != -1) {
        switch (opt) {
            case 'd':
                diagnostics_flag = true;
                break;
            default:
                break;
        }
    }

    if (diagnostics_flag) {
        system("./scripts/open_diagnostics.sh");
    }

    task_init();

    return 0;
}
