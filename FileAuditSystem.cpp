#include "FileAuditSystem.h"
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>

namespace fs = std::filesystem;

/* Public Methods */

FileAuditSystem::~FileAuditSystem() {
    this->thread_->join();
    delete this->thread_;
}

bool FileAuditSystem::startThread(const std::string& path) {
   auto path_to_directory = path;
   try {
        std::thread* thread_obj = new std::thread(&FileAuditSystem::startTimer, this, path_to_directory);
        this->thread_ = thread_obj;
        return true;
   }
   catch (const std::exception& e) {
        std::cout << "Error during threading process" << std::endl;
        std::cerr << e.what() << std::endl;
        return false;
    }
}

inline std::string FileAuditSystem::timestamp(const std::string& s) {
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    if (s == "now")
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    else if (s == "date")
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

void FileAuditSystem::stopThread() {
    flag = true;
}

/* Private Methods */

void FileAuditSystem::startTimer(const std::string& path) {
    while (!this->flag) {
        std::this_thread::sleep_for(std::chrono::seconds(20));
        logFileStatus(path);
    }
    std::cout << "Logging has finished." << '\n';
}

void FileAuditSystem::logFileStatus(const std::string& directory) {
    try {
        struct stat status;

        /* Check if the path entered is a directory */
        if (!fs::is_directory(directory)) {
            std::cout << "Path entered is not a directory.  Aborting program." << std::endl;
            exit(EXIT_SUCCESS);
        }

        /* Create a log file or appends existing file.  Log saved in directory where it's created. */
        std::string file_path = "/" + directory + "log_" + timestamp("date") + ".txt";
        std::ofstream ofs(file_path.c_str(), std::ios_base::out | std::ios_base::app);

        /* Iterate through the contents in the directory */
        for (const auto& p : fs::directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
            ofs << timestamp("now") << '\t' << getpid() << '\t' << status.st_uid << '\t'
                << ((fs::status(directory).permissions() & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
                << ((fs::status(directory).permissions() & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
                << ((fs::status(directory).permissions() & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
                << ((fs::status(directory).permissions() & fs::perms::group_read) != fs::perms::none ? "r" : "-")
                << ((fs::status(directory).permissions() & fs::perms::group_write) != fs::perms::none ? "w" : "-")
                << ((fs::status(directory).permissions() & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
                << ((fs::status(directory).permissions() & fs::perms::others_read) != fs::perms::none ? "r" : "-")
                << ((fs::status(directory).permissions() & fs::perms::others_write) != fs::perms::none ? "w" : "-")
                << ((fs::status(directory).permissions() & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
                << '\n';
        }
        ofs.close();
    }
    catch (const std::exception& e) {
        std::cout << "Error writing file status to log" << std::endl;
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}
