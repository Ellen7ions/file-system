//
// Created by Ellen7ions on 2021/12/19.
//

#include <FileBlockManager.hpp>


FileBlockManager::FileBlockManager(std::string filename, uint32_t size, uint8_t mode) : FileManager(filename, size, mode) {

}

void FileBlockManager::initBlock() {

}

void FileBlockManager::flushBlock(uint32_t offset) {
    fseek(this->curFp, offset * BLOCK_SIZE, SEEK_SET);
    char u = 0;
    for (int i = 0; i < BLOCK_SIZE; i++) fwrite(&u, sizeof(char), 1, this->curFp);
}

bool FileBlockManager::writeBlock(uint32_t block_sub, uint32_t offset, void *data, uint32_t size) {
    if ((block_sub < BLOCK_CNT) && (offset < BLOCK_SIZE)) {
        fseek(this->curFp, block_sub * BLOCK_SIZE + offset, SEEK_SET);
        fwrite(data, size, 1, this->curFp);
        return true;
    } else return false;
}

bool FileBlockManager::readBlock(uint32_t block_sub, uint32_t offset, void *data, uint32_t size) {
    if ((block_sub < BLOCK_CNT) && (offset < BLOCK_SIZE)) {
        fseek(this->curFp, block_sub * BLOCK_SIZE + offset, SEEK_SET);
        fread(data, size, 1, this->curFp);
        return true;
    } else return false;
}
