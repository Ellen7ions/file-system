//
// Created by Ellen7ions on 2021/12/19.
//

#ifndef COMPLEX_FILE_SYSTEM_FILEBLOCKMANAGER_HPP
#define COMPLEX_FILE_SYSTEM_FILEBLOCKMANAGER_HPP

#include "FileManager.hpp"
#include "config.h"

class FileBlockManager : public FileManager {
public:
    FileBlockManager(std::string filename, uint32_t size, uint8_t mode);

    void initBlock();

    bool writeBlock(uint32_t block_sub, uint32_t offset, void *data, uint32_t size);

    bool readBlock(uint32_t block_sub, uint32_t offset, void *data, uint32_t size);

    void flushBlock(uint32_t offset);
};


#endif //COMPLEX_FILE_SYSTEM_FILEBLOCKMANAGER_HPP
