//
// Created by Ellen7ions on 2021/12/19.
//

#ifndef COMPLEX_FILE_SYSTEM_FILEMANAGER_HPP
#define COMPLEX_FILE_SYSTEM_FILEMANAGER_HPP

#include <iostream>
#include <cstdint>
#include <cstdio>
#include "config.h"

class FileManager {
public:
    std::string filename{};
    uint32_t f_size;
    FILE *curFp;

    FileManager(std::string filename, uint32_t size, uint8_t mode);

    FILE *createEmptyFile(uint32_t size) const;

    void writeData(uint32_t offset, void *data, uint32_t size) const;

    void readData(uint32_t offset, void *data, uint32_t size) const;

    void clearFile() const;

    ~FileManager();
};


#endif //COMPLEX_FILE_SYSTEM_FILEMANAGER_HPP
