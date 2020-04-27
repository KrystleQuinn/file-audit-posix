#include "FileAuditSystem.h"
#include <iostream>
#include <signal.h>

bool signal_flagged = false;

void signalHandler(int single_) {
    if (single_ == SIGINT)
        signal_flagged = true;
}

int main() {
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        std::cerr << "Signal function registration failed." << std::endl;
        return EXIT_FAILURE;
    }

    FileAuditSystem fas;
    std::string directory_path;
    std::cout << "Please enter the full path to the directory you'd like to have audited: " << std::endl;
    std::getline(std::cin, directory_path);
    fas.setPath(directory_path);
    const auto directory = fas.getPath();

    if (!fas.startThread(directory)) {
        std::cout << "Could not start process." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Main thread is alive. Logging has started." << std::endl;
    std::cout << "To stop program press 'ctrl+c'" << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        if (signal_flagged) {
            std::cout << "\n";
            fas.stopThread();
            std::cout << "Main thread finished." << std::endl;
            return EXIT_SUCCESS;
        }
    }

    /* Never reached */
    return EXIT_SUCCESS;
}
