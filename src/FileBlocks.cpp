//
// Created by Ellen7ions on 2021/12/20.
//

#include "FileBlocks.hpp"

SuperBlock *superBlockLoad(uint8_t *data) {
    auto *block = (SuperBlock *) malloc(sizeof(SuperBlock));
    memcpy(block, data, sizeof(SuperBlock));
    return block;
}

SuperBlock *superBlockInit(uint32_t blocks_total_cnt, uint32_t empty_cnt, uint32_t cur_stack_top, uint32_t block_size) {
    auto *block = (SuperBlock *) malloc(sizeof(SuperBlock));
    block->blocks_total_cnt = blocks_total_cnt;
    block->empty_cnt = empty_cnt;
    block->cur_stack_top = cur_stack_top;
    block->block_size = block_size;
    for (int i = 0; i < 50; i++)
        block->stack[i] = i;
    return block;
}

IndexItem *indexItemInit(uint8_t fileType, uint32_t fileLength) {
    auto *indexItem = (IndexItem *) malloc(sizeof(IndexItem));
    indexItem->fileType = fileType;
    indexItem->fileLength = fileLength;
    indexItem->linkCnt = 1;
    memset(indexItem->psyAddr, 0, sizeof(uint16_t) * 13);
    return indexItem;
}

DirItem *dirItemInit(char *filename) {
    auto *dirItem = (DirItem *) malloc(sizeof(DirItem));
    strcpy(dirItem->filename, filename);
    return dirItem;
}

