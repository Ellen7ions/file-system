//
// Created by Ellen7ions on 2021/12/19.
//

#ifndef COMPLEX_FILE_SYSTEM_CONFIG_H
#define COMPLEX_FILE_SYSTEM_CONFIG_H

#define DEBUG

#define BLOCK_CNT   400
#define BLOCK_SIZE  1024

#define DISK_SIZE (BLOCK_CNT * BLOCK_SIZE)

#define DIR_BLOCK_CNT 10
#define INDEX_BLOCK_CNT 20

#define READ 0
#define CREATE 1

#endif //COMPLEX_FILE_SYSTEM_CONFIG_H
