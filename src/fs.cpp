
#include "ftree/fs.h"

FileSystem *file_system;

FileSystem **pFileSystem;

int top;

void fs_init(FileSystem *fs, uint8_t mode) {
    fs->disk = new VirtualDisk("test.bin", DISK_SIZE, CREATE);
    fs->root = fs->disk->buildFileTreeRoot();
    fs->cur_node = fs->root;
    // fs->root = (FileTreeNode *) kmalloc(sizeof(FileTreeNode));
    // fs->cur_node = fs->root;
    // strcpy(fs->root->file_name, "/");
    // fs->root->file_type = 'D';
    // fs->root->child = nullptr;
    // fs->root->sibling = nullptr;
    // fs->root->parent = fs->root;
}

FileTreeNode *fs_loc_node(const char *path) {
    if (strlen(path) == 0) return file_system->cur_node;
    int path_level;
    char *cpy_path = (char *) kmalloc(sizeof(char) * strlen(path));
    strcpy(cpy_path, path);
    char **paths = parse_path(cpy_path, &path_level);

    FileTreeNode *p;
    if (path[0] == '/') p = file_system->root;
    else p = file_system->cur_node;

    int i = 0;
    while (i < path_level && strcmp(p->file_name, paths[i]) != 0) {
        if (strcmp(paths[i], "..") == 0) {
            p = ftn_father(p);
            i += 1;
            continue;
        } else if (strcmp(paths[i], ".") == 0) {
            i += 1;
            continue;
        } else {
            p = p->child;
        }
        if (p == nullptr) goto finish;
        while (strcmp(p->file_name, paths[i]) != 0) {
            p = p->sibling;
            if (p == nullptr) goto finish;
        }
        i += 1;
    }
    finish:
    return p;
}

void fs_print_pwd(FileTreeNode *cur) {
    if (cur->parent == cur) {
        return;
    }
    fs_print_pwd(ftn_father(cur));
    printf("/%s", cur->file_name);
}

int fs_mkdir(const char *base_path, const char *dir_name) {
    FileTreeNode *path_node = fs_loc_node(base_path);
    if (path_node == nullptr) return -1;
    FileTreeNode *p = path_node->child;
    while (p != nullptr) {
        if (strcmp(p->file_name, dir_name) == 0) return 1;
        p = p->sibling;
    }
    FileTreeNode *ftn = ftn_new(dir_name, 'd', nullptr);
    ftn_add_node(path_node, ftn);
    ftn->index_i = file_system->disk->makeDir(path_node->index_i, (char *) dir_name,
                                              strcmp(path_node->file_name, "/") == 0);

    return 0;
}

int fs_rmdir(const char *path, int force) {
    FileTreeNode *path_node = fs_loc_node(path);
    if (path_node == nullptr) return -1;
    if ((force || path_node->child == nullptr) && path_node->file_type == 'd') {
        file_system->disk->removeDir(ftn_father(path_node)->index_i, path_node->file_name,
                                     strcmp((ftn_father(path_node))->file_name, "/") == 0);
        ftn_del_node(path_node);
        return 0;
    }
    return 1;
}

int fs_cd(const char *path) {
    FileTreeNode *path_node = fs_loc_node(path);
    if (path_node == nullptr || path_node->file_type == 'f') return -1;
    file_system->cur_node = path_node;
    FileTreeNode *t = path_node;
    file_system->cur_path = "";
    while (t->parent != t) {
        file_system->cur_path = "/" + std::string(t->file_name) + file_system->cur_path;
        t = ftn_father(t);
    }
    return 0;
}

int fs_ls(const char *path) {
    FileTreeNode *path_node = path == nullptr ? file_system->cur_node : fs_loc_node(path);
    if (path_node == nullptr) return -1;
    path_node = path_node->child;
    int cnt = 0;
    while (path_node != nullptr) {
        printf("[%c] %s\n", path_node->file_type, path_node->file_name);
        path_node = path_node->sibling;
        cnt += 1;
    }
    printf("total %d files.\n", cnt);
    return 0;
}

int fs_pwd() {
    FileTreeNode *path_node = file_system->cur_node;
    if (path_node == nullptr) return -1;
    fs_print_pwd(path_node);
    printf("\n");
    return 0;
}

int fs_creat(const char *path, const char *file_name, uint32_t file_size) {
    FileTreeNode *path_node = fs_loc_node(path);
    if (path_node == nullptr) return -1;
    FileTreeNode *p = path_node->child;
    while (p != nullptr) {
        if (strcmp(p->file_name, file_name) == 0) return 1;
        p = p->sibling;
    }
    ftn_add_node(path_node, ftn_new(file_name, 'f', nullptr));

    file_system->disk->makeFile(path_node->index_i, (char *) file_name, file_size,
                                strcmp(path_node->file_name, "/") == 0);
    return 0;
}

int fs_rm(const char *path, const char *file_name) {
    FileTreeNode *path_node = fs_loc_node(path);
    if (path_node == nullptr) return -1;
    FileTreeNode *p = path_node->child;
    while (p != nullptr) {
        if (strcmp(p->file_name, file_name) == 0) goto found;
        p = p->sibling;
    }
    return 1;
    found:
    if (p->file_type == 'f') {
        file_system->disk->removeFile(path_node->index_i, (char *) file_name, strcmp(path_node->file_name, "/") == 0);
        ftn_del_node(p);
    }
    return 0;
}

void ft_to_arr(FileTreeNode *root, FileTreeNode *arr, int index) {
    if (root == nullptr) return;
    arr[index] = *root;
    ft_to_arr(root->child, arr, 2 * index + 1);
    ft_to_arr(root->sibling, arr, 2 * index + 2);
}

void arr_to_ft(FileTreeNode **root, FileTreeNode *arr, int index, int arr_size, FileTreeNode *parent) {
    if (index >= arr_size || root == nullptr) return;
    *(root) = ftn_new(arr[index].file_name, arr[index].file_type, parent);
    if (arr[index].child != nullptr)
        arr_to_ft(&((*root)->child), arr, 2 * index + 1, arr_size, *(root));
    if (arr[index].sibling != nullptr)
        arr_to_ft(&((*root)->sibling), arr, 2 * index + 2, arr_size, *(root));
}

int fs_level(FileTreeNode *root) {
    if (root == nullptr) return 0;
    int l = fs_level(root->child);
    int r = fs_level(root->sibling);
    return (l > r ? l : r) + 1;
}

int fs_save(const char *file_tree_path) {
    int max_level = fs_level(file_system->root);
    int max_node = (1 << max_level) - 1;
    auto *ftn_arr = (FileTreeNode *) kmalloc(sizeof(FileTreeNode) * max_node);
    ft_to_arr(file_system->root, ftn_arr, 0);
    FILE *fp;
    fp = fopen(file_tree_path, "wb");
    if (fp == nullptr) {
        printf("FILE ERROR!\n");
        exit(-1);
    }
    fwrite(ftn_arr, sizeof(FileTreeNode), max_node, fp);

    fclose(fp);
    return 0;
}

int fs_reload(const char *file_tree_path) {
    FILE *fp;
    fp = fopen(file_tree_path, "rb");
    if (fp == nullptr) {
        printf("FILE ERROR!\n");
        exit(-1);
    }
    fseek(fp, 0, SEEK_END);
    int arr_size = ftell(fp) / sizeof(FileTreeNode);
    fseek(fp, 0, SEEK_SET);
    auto *ftn_arr = (FileTreeNode *) kmalloc(sizeof(FileTreeNode) * arr_size);
    fread(ftn_arr, sizeof(FileTreeNode), arr_size, fp);
    arr_to_ft(&(file_system->root), ftn_arr, 0, arr_size, file_system->root);
    file_system->cur_node = file_system->root;
    file_system->root->parent = file_system->root;
    fclose(fp);
    return 0;
}

int fs_vim(const char *path, const char *file_name) {
    FileTreeNode *path_node = fs_loc_node(path);
    if (path_node == nullptr) return -1;
    FileTreeNode *p = path_node->child;
    while (p != nullptr) {
        if (strcmp(p->file_name, file_name) == 0) goto found;
        p = p->sibling;
    }
    return 1;

    found:

    printf("[Vim]:\n");

    uint32_t blockIndex = file_system->disk->getFileFirstBlockIndex(path_node->index_i);

    char *buffer = (char *) malloc(sizeof(char) * 512);
    memset(buffer, 0, sizeof(char) * 512);

    uint32_t ptr = 0;
    while (true) {
        // printf("%d\n", ptr);
        gets(buffer);
        if (strcmp(buffer, "q") == 0) {
            buffer[0] = EOF;
            file_system->disk->fileBlockManager->writeBlock(blockIndex, ptr, buffer, sizeof(char));
            break;
        }
        uint32_t len = strlen(buffer);
        buffer[len] = '\n';
        file_system->disk->fileBlockManager->writeBlock(blockIndex, ptr, buffer, sizeof(char) * (len + 1));
        ptr += (len + 1);
        if (ptr >= 1024) {
            blockIndex += 1;
            ptr -= 1024;
        }
    }
    return 0;
}

int fs_cat(const char *path, const char *file_name) {
    FileTreeNode *path_node = fs_loc_node(path);
    if (path_node == nullptr) return -1;
    FileTreeNode *p = path_node->child;
    while (p != nullptr) {
        if (strcmp(p->file_name, file_name) == 0) goto found;
        p = p->sibling;
    }
    return 1;

    found:
    uint32_t blockIndex = file_system->disk->getFileFirstBlockIndex(path_node->index_i);

    char *buffer = (char *) malloc(sizeof(char) * 512);
    memset(buffer, 0, sizeof(char) * 512);
    uint32_t ptr = 0;
    while (true) {
        memset(buffer, 0, 512 * sizeof(char));
        file_system->disk->fileBlockManager->readBlock(blockIndex, ptr, buffer, sizeof(char) * 512);
        if (strlen(buffer) == 0) break;
        for (int i = 0; i < 512; i++) {
            if (buffer[i] == EOF) return 0;
            putchar(buffer[i]);
            ptr += 1;
        }
        puts(buffer);
        if (ptr >= 1024) {
            blockIndex += 1;
            ptr -= 1024;
        }
    }
    return 0;
}
