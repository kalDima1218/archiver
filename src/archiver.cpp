#include <string>
#include <iostream>
#include <stdexcept>
#include "archive.h"
#include "dearchive.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cerr << "Invalid command. Do ./archiver -h to get more info" << std::endl;
        return ErrorCode;
    }
    std::string command = static_cast<std::string>(argv[1]);
    if (command == "-c") {
        std::vector<std::string> file_names(argc - 3);
        for (int i = 3; i < argc; ++i) {
            file_names[i - 3] = static_cast<std::string>(argv[i]);
        }
        std::string archive_name = static_cast<std::string>(argv[2]);
        try {
            Archive(file_names, archive_name);
        } catch (std::exception& e) {
            std::cerr << e.what();
            return ErrorCode;
        }
    } else if (command == "-d") {
        std::string archive_name = static_cast<std::string>(argv[2]);
        try {
            Dearchive(archive_name);
        } catch (std::exception& e) {
            std::cerr << e.what();
            return ErrorCode;
        }
    } else if (command == "-h") {
        std::cout << "archiver -c archive_name file1 [file2 ...] - заархивировать файлы file1, file2, ... и сохранить "
                     "результат в файл archive_name"
                  << std::endl;
        std::cout << "archiver -d archive_name - разархивировать файлы из архива archive_name и положить в текущую "
                     "директорию."
                  << std::endl;
    } else {
        std::cerr << "invalid command" << std::endl;
        return ErrorCode;
    }
    return OkCode;
}
