//
// Created by Ellen7ions on 2021/12/20.
//

#include "DirBlockManager.hpp"

DirBlockManager::DirBlockManager(DirItem *ptr, uint32_t cnt) {
    this->dirItems = ptr;
    this->dirItemCnt = cnt;
}

bool DirBlockManager::addDirItem(char *filename, uint16_t index) {
    if (checkExists(filename)) return false;
    for (int i = 0; i < this->dirItemCnt; i++) {
        if (strlen(dirItems[i].filename) == 0) {
            strcpy(dirItems[i].filename, filename);
            dirItems[i].index_i = index;
        }
    }
    return true;
}

bool DirBlockManager::checkExists(char *filename) {
    for (int i = 0; i < this->dirItemCnt; i++) {
        if (strcmp(dirItems[i].filename, filename) == 0)
            return true;
    }
    return false;
}

uint16_t DirBlockManager::removeDirItem(char *filename, bool *flag) {
    if (!checkExists(filename)) {
        *flag = false;
        return 0;
    }
    *flag = true;
    uint16_t res;
    for (int i = 0; i < this->dirItemCnt; i++) {
        if (strcmp(this->dirItems[i].filename, filename) == 0) {
            res = this->dirItems[i].index_i;
            memset(this->dirItems + i, 0, sizeof(DirItem));
            return res;
        }
    }
    return 0;
}

uint16_t DirBlockManager::getNodeIndex(char *filename, bool *flag) {
    if (!checkExists(filename)) {
        *flag = false;
        return 0;
    }
    *flag = true;
    for (int i = 0; i < this->dirItemCnt; i++) {
        if (strcmp(this->dirItems[i].filename, filename) == 0) {
            return this->dirItems[i].index_i;
        }
    }
    return 0;
}

bool DirBlockManager::renameDirItem(char *filename) {
    if (!checkExists(filename)) return false;
    for (int i = 0; i < this->dirItemCnt; i++) {
        if (strcmp(this->dirItems[i].filename, filename) == 0) {
            strcpy(this->dirItems[i].filename, filename);
        }
    }
    return true;
}
