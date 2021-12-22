#ifndef _FS_H_
#define _FS_H_

#include "cstring"
#include "ftn.h"
#include "VirtualDisk.hpp"

typedef struct FileSystem {
    FileTreeNode *root;
    FileTreeNode *cur_node;

    std::string cur_path;

    VirtualDisk *disk;

    FileSystem(std::string filename, uint32_t size, uint8_t mode) {
        this->disk = new VirtualDisk(filename, size, mode);

        this->root = this->disk->buildFileTreeRoot();
        this->cur_node = this->root;
        this->cur_path = "/";
    }

} FileSystem;

extern FileSystem *file_system;

/**
 * Init a file system.
 * @param fs
 */
void fs_init(FileSystem *fs);

// utils
FileTreeNode *fs_loc_node(const char *path);

void fs_print_pwd(FileTreeNode *cur);

// cmds
int fs_mkdir(const char *base_path, const char *dir_name);

int fs_rmdir(const char *path, int force);

int fs_cd(const char *path);

int fs_ls(const char *path);

int fs_pwd();

int fs_creat(const char *path, const char *file_name, uint32_t file_size);

int fs_rm(const char *path, const char *file_name);

int fs_save(const char *file_tree_path);

int fs_reload(const char *file_tree_path);

int fs_vim(const char *path, const char *file_name);

int fs_cat(const char *path, const char *file_name);

int fs_upload(const char *file_name);

int fs_download(const char *file_name, const char *output_path);

void ft_to_arr(FileTreeNode *root, FileTreeNode *arr, int index);

void arr_to_ft(FileTreeNode **root, FileTreeNode *arr, int index, int arr_size, FileTreeNode *parent);

int fs_level(FileTreeNode *root);

#endif