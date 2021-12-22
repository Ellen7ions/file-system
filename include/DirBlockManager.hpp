//
// Created by Ellen7ions on 2021/12/20.
//

#ifndef COMPLEX_FILE_SYSTEM_DIRBLOCKMANAGER_HPP
#define COMPLEX_FILE_SYSTEM_DIRBLOCKMANAGER_HPP

#include "cstring"
#include "FileBlocks.hpp"

class DirBlockManager {
private:
    DirItem *dirItems;
    uint32_t dirItemCnt;

public:
    DirBlockManager(DirItem *ptr, uint32_t cnt);

    bool checkExists(char *filename);

    bool addDirItem(char *filename, uint16_t index);

    uint16_t removeDirItem(char *filename, bool *flag);

    uint16_t getNodeIndex(char *filename, bool *flag);

    bool renameDirItem(char *filename);
};


#endif //COMPLEX_FILE_SYSTEM_DIRBLOCKMANAGER_HPP
