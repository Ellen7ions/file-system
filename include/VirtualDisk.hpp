//
// Created by Ellen7ions on 2021/12/20.
//

#ifndef COMPLEX_FILE_SYSTEM_VIRTUALDISK_HPP
#define COMPLEX_FILE_SYSTEM_VIRTUALDISK_HPP

#include "cstring"

#include "FileBlockManager.hpp"
#include "FileBlocks.hpp"
#include "DirBlockManager.hpp"
#include "PathUtils.h"

#include "ftree/ftn.h"

class VirtualDisk {
public:
    FileBlockManager *fileBlockManager;

    uint32_t diskTotalSize;
    uint32_t blockTotalCnt;
    uint32_t emptyCnt;

    uint32_t dirItemCnt;
    uint32_t indexItemCnt;

    SuperBlock *superBlock;
    DirItem *dirItems;
    IndexItem *indexItems;

    // DirBlockManager *dirBlockManager;

    VirtualDisk(std::string filename, uint32_t size, uint8_t mode);

    ~VirtualDisk();

    void initDisk();

    void initDataBlock();

    void formatDisk();

    void diskClose();

    void writeBack() const;

    uint32_t mallocBlock();

    void freeBlock(uint32_t);

    // File Functions

    uint32_t createFileIndex(IndexItem *indexItem);

    void createDirItem(DirItem *dirItem, bool load = false);

    void createRoot();

    // Disk -> FTN
    FileTreeNode *buildFileTreeRoot();

    FileTreeNode *buildLevelDir(FileTreeNode *parent, const char *curFilename, uint32_t curIndex_i);

    FileTreeNode *getNodeFromDirItem(uint32_t sub) const;

    uint32_t makeDir(uint32_t parentIndex, char *dirName, bool atRootDir = false);

    uint32_t makeFile(uint32_t parentIndex, char *fileName, uint32_t fileSize, bool atRootDir);

    void removeDir(uint32_t parentIndex, char *dirName, bool atRootDur = false);

    void removeFile(uint32_t parentIndex, char *fileName, bool atRootDur = false);

    uint32_t getFileFirstBlockIndex(uint32_t index_i) const;



    // void removeFile(uint32_t parentIndex, char *dirName, bool atRootDur = false);

    // uint16_t dirBlockFindPath(uint32_t index, std::vector<std::string> vec, int subIndex);

    // uint16_t findPathIndex(std::string path);

#ifdef DEBUG

    void displayBlock(bool onlyStack);

#endif
};


#endif //COMPLEX_FILE_SYSTEM_VIRTUALDISK_HPP
