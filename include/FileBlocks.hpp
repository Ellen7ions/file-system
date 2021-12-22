//
// Created by Ellen7ions on 2021/12/19.
//

#ifndef COMPLEX_FILE_SYSTEM_FILEBLOCKS_HPP
#define COMPLEX_FILE_SYSTEM_FILEBLOCKS_HPP

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>

typedef struct HeaderBlock {

} HeaderBlock;

typedef struct SuperBlock {
    uint32_t blocks_total_cnt;
    uint32_t empty_cnt;
    uint32_t cur_stack_top;
    uint32_t block_size;
    uint32_t stack[50];
} SuperBlock;

SuperBlock *superBlockLoad(uint8_t *data);

SuperBlock *superBlockInit(uint32_t blocks_total_cnt,
                           uint32_t empty_cnt,
                           uint32_t cur_stack_top,
                           uint32_t block_size);

typedef struct DirItem {
    char filename[14];
    uint16_t index_i;
} DirItem;

DirItem* dirItemInit(char *filename);

#pragma pack(1)
typedef struct IndexItem {
    uint8_t fileType;
    uint32_t fileLength;
    uint8_t linkCnt;
    uint16_t psyAddr[13];
} IndexItem;
#pragma pack()

IndexItem *indexItemInit(uint8_t fileType,
                         uint32_t fileLength);


#endif //COMPLEX_FILE_SYSTEM_FILEBLOCKS_HPP
