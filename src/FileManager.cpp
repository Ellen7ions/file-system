//
// Created by Ellen7ions on 2021/12/19.
//

#include "FileManager.hpp"

FileManager::FileManager(std::string filename, uint32_t size, uint8_t mode) {
    this->filename = std::move(filename);

    if (mode == CREATE) this->curFp = createEmptyFile(size);
    else if (mode == READ) {
        this->curFp = fopen(this->filename.c_str(), "rb+");
    }
    this->f_size = size;
}

FILE *FileManager::createEmptyFile(uint32_t size) const {
    FILE *fp = fopen(this->filename.c_str(), "w");
    for (int i = 0; i < size; i++)
        fprintf(fp, "%c", 0);
    fclose(fp);
    fp = fopen(this->filename.c_str(), "rb+");
    return fp;
}

void FileManager::writeData(uint32_t offset, void *data, uint32_t size) const {
    if (this->curFp == nullptr) return;
    fseek(this->curFp, offset, SEEK_SET);
    fwrite(data, size, 1, this->curFp);
}

void FileManager::readData(uint32_t offset, void *data, uint32_t size) const {
    if (this->curFp == nullptr) return;
    fseek(this->curFp, offset, SEEK_SET);
    fread(data, size, 1, this->curFp);
}

void FileManager::clearFile() const {
    FILE *fp = fopen(this->filename.c_str(), "w");
    for (int i = 0; i < this->f_size; i++)
        fprintf(fp, "%c", 0);
    fclose(fp);
    fclose(this->curFp);
}

FileManager::~FileManager() {
    fclose(this->curFp);
}
