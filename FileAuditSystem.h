#ifndef FILEAUDITSYSTEM_FILEAUDITSYSTEM_H
#define FILEAUDITSYSTEM_FILEAUDITSYSTEM_H

#include <string>
#include <filesystem>
#include <thread>
#include <functional>


namespace fs = std::filesystem;

class FileAuditSystem {
private:
    std::string path_;
    bool flag = false;
    std::thread* thread_;

    void startTimer(const std::string& path);

    void logFileStatus(const std::string& path);

public:
    void setPath(std::string path) {
        path_ = path;
    }

    std::string getPath() const {
        return path_;
    }

    ~FileAuditSystem();

    std::string timestamp(const std::string& s);

    bool startThread(std::string path);

    void stopThread();
};


#endif //FILEAUDITSYSTEM_FILEAUDITSYSTEM_H
