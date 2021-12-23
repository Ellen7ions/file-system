//
// Created by Ellen7ions on 2021/12/20.
//

#include <VirtualDisk.hpp>

#include <utility>
#include <vector>


VirtualDisk::VirtualDisk(std::string filename, uint32_t size, uint8_t mode) {
    this->fileBlockManager = new FileBlockManager(std::move(filename), size, mode);

    this->diskTotalSize = size;
    this->dirItemCnt = BLOCK_SIZE * DIR_BLOCK_CNT / sizeof(DirItem);
    this->indexItemCnt = BLOCK_SIZE * INDEX_BLOCK_CNT / sizeof(IndexItem);

    if (mode == READ) {
        auto *data = (uint8_t *) malloc(BLOCK_SIZE);
        this->fileBlockManager->readBlock(1, 0, data, BLOCK_SIZE);
        this->superBlock = superBlockLoad(data);
        this->blockTotalCnt = this->superBlock->blocks_total_cnt;
        this->emptyCnt = this->superBlock->empty_cnt;

        // DirBlock
        this->dirItems = (DirItem *) malloc(BLOCK_SIZE * DIR_BLOCK_CNT);
        for (int i = 0; i < this->dirItemCnt; i += (BLOCK_SIZE / sizeof(DirItem))) {
            memset(this->dirItems + i, 0, BLOCK_SIZE);
            this->fileBlockManager->readBlock(2 + (i / (BLOCK_SIZE / sizeof(DirItem))), 0, this->dirItems + i,
                                              BLOCK_SIZE);
        }

        // IndexBlock
        this->indexItems = (IndexItem *) malloc(BLOCK_SIZE * INDEX_BLOCK_CNT);
        for (int i = 0; i < this->indexItemCnt; i++) {
            memset(this->indexItems + i, 0, sizeof(IndexItem));
            this->fileBlockManager->readBlock(12 + (i / (BLOCK_SIZE / sizeof(IndexItem))),
                                              (i % (BLOCK_SIZE / sizeof(IndexItem))) * sizeof(IndexItem),
                                              this->indexItems + i,
                                              sizeof(IndexItem));
        }
    } else if (mode == CREATE) {
        formatDisk();

        // createRoot();
    }
    // this->superBlock = (SuperBlock *) malloc(sizeof(SuperBlock));
}


void VirtualDisk::formatDisk() {
    initDisk();

    // SuperBlock
    this->superBlock = superBlockInit(this->blockTotalCnt, this->emptyCnt, 50, BLOCK_SIZE);

    // DirBlock
    this->dirItems = (DirItem *) malloc(BLOCK_SIZE * DIR_BLOCK_CNT);
    memset(this->dirItems, 0, BLOCK_SIZE * DIR_BLOCK_CNT);

    // IndexBlock
    this->indexItems = (IndexItem *) malloc(BLOCK_SIZE * INDEX_BLOCK_CNT);
    memset(this->indexItems, 0, BLOCK_SIZE * INDEX_BLOCK_CNT);

    // this->dirBlockManager = new DirBlockManager(this->dirItems, this->dirItemCnt);

    initDataBlock();

    writeBack();
}

void VirtualDisk::initDisk() {
    this->blockTotalCnt = this->diskTotalSize / BLOCK_SIZE;
    this->emptyCnt = this->blockTotalCnt - 51;
}

void VirtualDisk::writeBack() const {
    // LeadBlock
    // this->fileBlockManager->flushBlock(0);

    // SuperBlock
    // this->fileBlockManager->writeData(1, this->superBlock, sizeof(SuperBlock));
    this->fileBlockManager->flushBlock(1);
    this->fileBlockManager->writeBlock(1, 0, this->superBlock, sizeof(SuperBlock));

    // DirBlock
    for (int i = 0; i < 10; i++) this->fileBlockManager->flushBlock(2 + i);
    for (int i = 0; i < this->dirItemCnt; i++) {
        this->fileBlockManager->writeBlock(2 + (i / (BLOCK_SIZE / sizeof(DirItem))),
                                           (i % (BLOCK_SIZE / sizeof(DirItem))) * sizeof(DirItem),
                                           this->dirItems + i,
                                           sizeof(DirItem));
    }

    // IndexBlock
    for (int i = 0; i < 20; i++) this->fileBlockManager->flushBlock(12 + i);
    for (int i = 0; i < this->indexItemCnt; i++) {
        this->fileBlockManager->writeBlock(12 + (i / (BLOCK_SIZE / sizeof(IndexItem))),
                                           (i % (BLOCK_SIZE / sizeof(IndexItem))) * sizeof(IndexItem),
                                           this->indexItems + i,
                                           sizeof(IndexItem));
    }
}

void VirtualDisk::initDataBlock() {
    // 32
    uint32_t i = 100;

    for (int k = 0; k < 50; k++) {
        this->superBlock->stack[k] = 51 + k;
    }
    this->superBlock->cur_stack_top = 50;

    while (true) {
        if (i + 50 >= BLOCK_CNT) {
            uint32_t free_num = 49;
            this->fileBlockManager->writeBlock(i, 0, &free_num, sizeof(uint32_t));
            uint32_t temp = 0;
            this->fileBlockManager->writeBlock(i, sizeof(uint32_t) * 1, &temp, sizeof(uint32_t));
            for (int j = 0; j <= free_num; j++) {
                uint32_t t_num = i + 1 + j;
                this->fileBlockManager->writeBlock(i, sizeof(uint32_t) * (2 + j), &t_num, sizeof(uint32_t));
            }
            break;
        } else {
            uint32_t free_num = 50;
            this->fileBlockManager->writeBlock(i, 0, &free_num, sizeof(uint32_t));
            for (int j = 0; j < 50; j++) {
                uint32_t t_num = i + 1 + j;
                this->fileBlockManager->writeBlock(i, (j + 1) * sizeof(uint32_t), &t_num, sizeof(uint32_t));
            }
        }
        i += 50;

    }
}

#ifdef DEBUG

void VirtualDisk::displayBlock(bool onlyStack) {
    std::cout << "Block index items:" << std::endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < (this->indexItems[i].fileLength / BLOCK_SIZE + 1); j++)
            std::cout << this->indexItems[i].psyAddr[j] << " ";
        std::cout << std::endl;
    }
    std::cout << "Block dir items:" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << this->dirItems[i].filename << ", " << this->dirItems[i].index_i << std::endl;
    }
    std::cout << "Block dir items:" << std::endl;
    std::cout << "Stack top:" << " " << this->superBlock->cur_stack_top << std::endl;
    if (onlyStack) return;
    std::cout << "=======================================" << std::endl;
    for (int i = 100; i < BLOCK_CNT; i += 50) {
        uint32_t free_num;
        this->fileBlockManager->readBlock(i, 0, &free_num, sizeof(uint32_t));
        std::cout << "N:" << free_num << std::endl;
        for (int j = 0; j < 50; j++) {
            this->fileBlockManager->readBlock(i, sizeof(uint32_t) * (j + 1), &free_num, sizeof(uint32_t));
            std::cout << free_num << std::endl;
        }

        std::cout << "=======================================" << std::endl;
    }
}

VirtualDisk::~VirtualDisk() = default;

uint32_t VirtualDisk::mallocBlock() {
    this->emptyCnt -= 1;

    uint32_t top_block_num = this->superBlock->stack[50 - this->superBlock->cur_stack_top];
    if (this->superBlock->cur_stack_top == 1) {
        this->fileBlockManager->readBlock(top_block_num, 0, &(this->superBlock->cur_stack_top), sizeof(uint32_t));
        this->fileBlockManager->readBlock(top_block_num, sizeof(uint32_t), this->superBlock->stack,
                                          this->superBlock->cur_stack_top * sizeof(uint32_t));
    } else {
        this->superBlock->cur_stack_top -= 1;
    }
    return top_block_num;
}

void VirtualDisk::diskClose() {
    writeBack();
}

void VirtualDisk::freeBlock(uint32_t new_sub) {
    this->emptyCnt += 1;
    this->fileBlockManager->flushBlock(new_sub);
    if (this->superBlock->cur_stack_top == 50) {
        this->fileBlockManager->writeBlock(new_sub, 0, &this->superBlock->cur_stack_top, sizeof(uint32_t));
        this->fileBlockManager->writeBlock(new_sub, sizeof(uint32_t), this->superBlock->stack,
                                           this->superBlock->cur_stack_top * sizeof(uint32_t));
        this->superBlock->cur_stack_top = 1;
        this->superBlock->stack[50 - this->superBlock->cur_stack_top - 1] = new_sub;
    } else {
        this->superBlock->stack[50 - this->superBlock->cur_stack_top - 1] = new_sub;
        this->superBlock->cur_stack_top += 1;
    }
}

uint32_t VirtualDisk::createFileIndex(IndexItem *indexItem) {
    uint32_t locAddrs[512];
    uint32_t top = 0;

    if (indexItem->fileType == 'd') {
        for (int i = 0; i < 4; i++) locAddrs[top++] = mallocBlock();
    } else if (indexItem->fileType == 'f') {
        uint32_t block_cnt = (indexItem->fileLength / BLOCK_SIZE) + 1;
        if (block_cnt <= 10) {
            for (int i = 0; i < block_cnt; i++) locAddrs[top++] = mallocBlock();
        } else {
#ifndef TWO_LEVEL_INDEX
            std::cout << "This file is so big! XD" << std::endl;
#else
            for (int i = 0; i < 10; i++) locAddrs[top++] = mallocBlock();
            IndexItem *another = indexItemInit(indexItem->fileType, indexItem->fileLength - 10 * BLOCK_SIZE);
            uint32_t index = createFileIndex(another);
            if (index == 0) {
                std::cout << "This file is so big! XD" << std::endl;
                return 0;
            }
            locAddrs[top++] = index;
#endif
        }
    }

    int32_t freeIndex;
    for (freeIndex = 1; freeIndex < this->indexItemCnt; freeIndex++) {
        if (this->indexItems[freeIndex].fileType == 0) {
            for (int i = 0; i < top; i++) {
                indexItem->psyAddr[i] = locAddrs[i];
                // std::cout << i << " alloc block index: " << locAddrs[i] << "\n";
            }

            memcpy(this->indexItems + freeIndex, indexItem, sizeof(IndexItem));
            this->fileBlockManager->writeBlock(
                    12 + (freeIndex / (BLOCK_SIZE / sizeof(IndexItem))),
                    (freeIndex % (BLOCK_SIZE / sizeof(IndexItem))) * sizeof(IndexItem),
                    indexItem,
                    sizeof(IndexItem)
            );
            return freeIndex;
        }
    }
    for (int i = 0; i < top; i++) freeBlock(locAddrs[i]);
    return 0;
}

void VirtualDisk::createDirItem(DirItem *dirItem, bool load) {
    int32_t i;
    for (i = 0; i < (DIR_BLOCK_CNT * BLOCK_SIZE / sizeof(DirItem)); i++) {
        if (strlen(this->dirItems[i].filename) == 0) {
            if (!load) {
                IndexItem *dirIndexItem = indexItemInit('d', 1);
                dirItem->index_i = createFileIndex(dirIndexItem);
            }

            memcpy(this->dirItems + i, dirItem, sizeof(DirItem));
            return;
        }
    }
}

void VirtualDisk::createRoot() {
    IndexItem *indexItem;
    DirItem *dirItem;

    dirItem = dirItemInit((char *) "bin");
    createDirItem(dirItem);

    dirItem = dirItemInit((char *) "home");
    createDirItem(dirItem);

    dirItem = dirItemInit((char *) "usr");
    createDirItem(dirItem);

    indexItem = indexItemInit('f', 64);
    uint32_t node_i = createFileIndex(indexItem);

    dirItem = dirItemInit((char *) "HelloWorld.c");
    dirItem->index_i = node_i;
    createDirItem(dirItem, true);
}

FileTreeNode *VirtualDisk::buildFileTreeRoot() {
    auto *root = ftn_new("/", 'd', nullptr);
    root->parent = root;
    for (int i = 0; i < BLOCK_SIZE / (sizeof(IndexItem)); i++) {
        if (strlen(dirItems[i].filename) != 0) {
            IndexItem ch = this->indexItems[dirItems[i].index_i];
            if (ch.fileType == 'd') {
                // a directory in the root, build a dir node linked with root, build its children.
                buildLevelDir(root, dirItems[i].filename, dirItems[i].index_i);
            } else if (ch.fileType == 'f') {
                // a file node
                FileTreeNode *fileNode = getNodeFromDirItem(i);
                ftn_add_node(root, fileNode);
            }
        }
    }
    return root;
}

FileTreeNode *VirtualDisk::getNodeFromDirItem(uint32_t sub) const {
    DirItem dirItem = this->dirItems[sub];
    IndexItem indexItem = this->indexItems[dirItem.index_i];
    FileTreeNode *node = ftn_new(dirItem.filename, indexItem.fileType, nullptr);
    node->index_i = dirItem.index_i;
    return node;
}

FileTreeNode *VirtualDisk::buildLevelDir(FileTreeNode *parent, const char *curFilename, uint32_t curIndex_i) {
    IndexItem indexItem = this->indexItems[curIndex_i];
    if (indexItem.fileType == 'f') return nullptr;
    // though alloc 4 block, I only read the first one.
    // because Im lazy :D
    auto *blockDirItems = (DirItem *) malloc(BLOCK_SIZE);
    this->fileBlockManager->readBlock(indexItem.psyAddr[0], 0, blockDirItems, BLOCK_SIZE);
    FileTreeNode *thisRoot = ftn_new(curFilename, 'd', parent);
    thisRoot->index_i = curIndex_i;
    for (int i = 0; i < BLOCK_SIZE / (sizeof(DirItem)); i++) {
        DirItem curDir = blockDirItems[i];
        if (strlen(curDir.filename) != 0) {
            IndexItem curIndex = this->indexItems[curDir.index_i];
            if (curIndex.fileType == 'd') {
                buildLevelDir(
                        thisRoot,
                        curDir.filename,
                        curDir.index_i
                );
                // ftn_add_node(thisRoot, ptr);
            } else if (curIndex.fileType == 'f') {
                FileTreeNode *fileNode = ftn_new(
                        curDir.filename,
                        'f',
                        nullptr
                );
                fileNode->index_i = curDir.index_i;
                ftn_add_node(thisRoot, fileNode);
            }
        }
    }
    ftn_add_node(parent, thisRoot);
    return thisRoot;
}

uint32_t VirtualDisk::makeDir(uint32_t parentIndex, char *dirName, bool atRootDir) {
    IndexItem *newIndexItem = indexItemInit('d', 0);
    uint32_t dirIndex = createFileIndex(newIndexItem);

    uint32_t psyAddr;
    if (atRootDir) {
        psyAddr = 2;
    } else {
        IndexItem parentIndexItem = this->indexItems[parentIndex];
        psyAddr = parentIndexItem.psyAddr[0];
    }

    auto *blockDirItems = (DirItem *) malloc(BLOCK_SIZE);
    this->fileBlockManager->readBlock(psyAddr, 0, blockDirItems, BLOCK_SIZE);

    uint32_t i;
    for (i = 0; i < this->dirItemCnt; i++) {
        if (strlen(blockDirItems[i].filename) == 0) break;
    }

    DirItem *dirItem = dirItemInit(dirName);
    dirItem->index_i = dirIndex;

    this->fileBlockManager->writeBlock(psyAddr, i * (sizeof(DirItem)), dirItem, sizeof(DirItem));
    if (atRootDir) {
        memcpy(this->dirItems + i, dirItem, sizeof(DirItem));
    }
    return dirIndex;
}

uint32_t VirtualDisk::makeFile(uint32_t parentIndex, char *fileName, uint32_t fileSize, bool atRootDir) {
    IndexItem *newIndexItem = indexItemInit('f', fileSize);
    uint32_t fileIndex = createFileIndex(newIndexItem);

    uint32_t psyAddr;
    if (atRootDir) {
        psyAddr = 2;
    } else {
        IndexItem parentIndexItem = this->indexItems[parentIndex];
        psyAddr = parentIndexItem.psyAddr[0];
    }

    auto *blockDirItems = (DirItem *) malloc(BLOCK_SIZE);
    this->fileBlockManager->readBlock(psyAddr, 0, blockDirItems, BLOCK_SIZE);

    uint32_t i;
    for (i = 0; i < BLOCK_SIZE / sizeof(DirItem); i++) {
        if (strlen(blockDirItems[i].filename) == 0) break;
    }

    DirItem *dirItem = dirItemInit(fileName);
    dirItem->index_i = fileIndex;

    this->fileBlockManager->writeBlock(psyAddr, i * (sizeof(DirItem)), dirItem, sizeof(DirItem));
    if (atRootDir) {
        memcpy(this->dirItems + i, dirItem, sizeof(DirItem));
    }
    return fileIndex;
}

void VirtualDisk::removeDir(uint32_t parentIndex, char *dirName, bool atRootDir) {
    printf("removeDir %d, %s\n", parentIndex, dirName);
    uint32_t psyAddr;
    if (atRootDir) {
        psyAddr = 2;
    } else {
        IndexItem parentIndexItem = this->indexItems[parentIndex];
        psyAddr = parentIndexItem.psyAddr[0];
    }

    auto *blockDirItems = (DirItem *) malloc(BLOCK_SIZE);
    this->fileBlockManager->readBlock(psyAddr, 0, blockDirItems, BLOCK_SIZE);

    uint32_t i;
    for (i = 0; i < BLOCK_SIZE / sizeof(DirItem); i++) {
        if ((strcmp(blockDirItems[i].filename, dirName) == 0) ||
            ((strcmp(dirName, "-1") == 0) && (strlen(blockDirItems[i].filename) != 0))) {
            // removeDir(blockDirItems[i].index_i, (char *) "-1", false);
            void *empty;

            // free block
            IndexItem *dirIndexItem = this->indexItems + blockDirItems[i].index_i;
            for (int j = 0; j < 4; j++) {
                printf("index: %d, free block num: %d\n", blockDirItems[i].index_i, dirIndexItem->psyAddr[j]);
                this->freeBlock(dirIndexItem->psyAddr[j]);
            }

            // clean indexItems

            empty = new IndexItem;
            memset(empty, 0, sizeof(IndexItem));
            this->fileBlockManager->writeBlock(12,
                                               blockDirItems[i].index_i * sizeof(IndexItem),
                                               empty, sizeof(IndexItem));
            memset(this->indexItems + blockDirItems[i].index_i, 0, sizeof(IndexItem));

            // clean dirItems
            empty = new DirItem;
            memset(empty, 0, sizeof(DirItem));
            this->fileBlockManager->writeBlock(psyAddr, i * sizeof(DirItem), empty, sizeof(DirItem));
            if (atRootDir) {
                memset(this->dirItems + i, 0, sizeof(DirItem));
            }
        }
    }
}

void VirtualDisk::removeFile(uint32_t parentIndex, char *fileName, bool atRootDir) {
    printf("removeFile %d, %s\n", parentIndex, fileName);
    uint32_t psyAddr;
    if (atRootDir) {
        psyAddr = 2;
    } else {
        IndexItem parentIndexItem = this->indexItems[parentIndex];
        psyAddr = parentIndexItem.psyAddr[0];
    }

    auto *blockDirItems = (DirItem *) malloc(BLOCK_SIZE);
    this->fileBlockManager->readBlock(psyAddr, 0, blockDirItems, BLOCK_SIZE);

    uint32_t i;
    for (i = 0; i < BLOCK_SIZE / sizeof(DirItem); i++) {
        if ((strcmp(blockDirItems[i].filename, fileName) == 0) ||
            ((strcmp(fileName, "-1") == 0) && (strlen(blockDirItems[i].filename) != 0))) {
            // removeDir(blockDirItems[i].index_i, (char *) "-1", false);
            void *empty;

            // free block
            IndexItem *dirIndexItem = this->indexItems + blockDirItems[i].index_i;

            for (int j = 0; dirIndexItem->psyAddr[j] != 0; j++) {
                printf("index: %d, free block num: %d\n", blockDirItems[i].index_i, dirIndexItem->psyAddr[j]);
                this->freeBlock(dirIndexItem->psyAddr[j]);
            }

            // clean indexItems

            empty = new IndexItem;
            memset(empty, 0, sizeof(IndexItem));
            this->fileBlockManager->writeBlock(12,
                                               blockDirItems[i].index_i * sizeof(IndexItem),
                                               empty, sizeof(IndexItem));
            memset(this->indexItems + blockDirItems[i].index_i, 0, sizeof(IndexItem));

            // clean dirItems
            empty = new DirItem;
            memset(empty, 0, sizeof(DirItem));
            this->fileBlockManager->writeBlock(psyAddr, i * sizeof(DirItem), empty, sizeof(DirItem));
            if (atRootDir) {
                memset(this->dirItems + i, 0, sizeof(DirItem));
            }
        }
    }
}

uint32_t VirtualDisk::getFileFirstBlockIndex(uint32_t index_i) const {
    return this->indexItems[index_i].psyAddr[0];
}


#endif
