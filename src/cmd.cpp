#include "ftree/cmd.h"

char *fs_cmds[] = {
        (char *) "mkdir",
        (char *) "rmdir",
        (char *) "cd",
        (char *) "ls",
        (char *) "pwd",
        (char *) "creat",
        (char *) "rm",
        (char *) "save",
        (char *) "reload",
        (char *) "menu",
        (char *) "quit",
        (char *) "vim",
        (char *) "cat",
        (char *) "upload",
        (char *) "download",
        (char *) "tree",
        (char *) "info"
};

int cmds_len = 17;

int (*sys_cmds[])(const int *argc, char **argv) = {
        cmd_mkdir,
        cmd_rmdir,
        cmd_cd,
        cmd_ls,
        cmd_pwd,
        cmd_creat,
        cmd_rm,
        cmd_save,
        cmd_reload,
        cmd_menu,
        cmd_quit,
        cmd_vim,
        cmd_cat,
        cmd_upload,
        cmd_download,
        cmd_tree,
        cmd_info
};

int cmd_mkdir(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    if (*argc <= 1) return -1;
    char *arg_path = argv[1];
    char base_path[64];
    char dir_path[64];
    split_path(arg_path, base_path, dir_path);
    int flag = fs_mkdir(base_path, dir_path);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("Deduplicated dir name!\n");
            break;
        default:
            break;
    }
    return 0;
}

int cmd_rmdir(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    if (*argc <= 1) return -1;
    char *arg_path = argv[1];
    char base_path[64];
    char dir_path[64];
    split_path(arg_path, base_path, dir_path);
    int flag = fs_rmdir(dir_path, (*argc == 3) && (strcmp(argv[2], "-f") == 0));
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("Dir name doesn't exists or Dir is not empty!\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_cd(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    if (*argc <= 1) return -1;
    char *arg_path = argv[1];
    int flag = fs_cd(arg_path);
    if (flag == -1) {
        printf("Error path !\n");
        return -1;
    }
    return flag;
}

int cmd_ls(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *arg_path = *argc == 1 ? NULL : argv[1];
    return fs_ls(arg_path);
}

int cmd_pwd(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    return fs_pwd();
}

int cmd_creat(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *arg_path = argv[1];
    char base_path[64];
    char file_path[64];
    uint32_t file_size = (*argc == 3) ? atoi(argv[2]) : 64;
    split_path(arg_path, base_path, file_path);
    int flag = fs_creat(base_path, file_path, file_size);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("Deduplicated file name!\n\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_rm(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *arg_path = argv[1];
    char base_path[64];
    char file_path[64];
    split_path(arg_path, base_path, file_path);
    int flag = fs_rm(base_path, file_path);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("File name doesn't exists!\n\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_save(const int *argc, char **argv) {
    if (*argc == 0) return -1;

    char *dir_path = *argc == 1 ? (char *) "file_tree.bin" : argv[1];
    return fs_save(dir_path);
}

int cmd_reload(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *dir_path = *argc == 1 ? (char *) "file_tree.bin" : argv[1];
    return fs_reload(dir_path);
}

int cmd_menu(const int *argc, char **argv) {
    if (*argc == 0) return -1;
#define SHOW_LINE(CMD, INTRO) printf("%-15s\t%-60s\n", #CMD, #INTRO)
    SHOW_LINE(,);
    SHOW_LINE([rmdir], Remove a directory.(rmdir dir_name));
    SHOW_LINE([mkdir], Make a directory.(mkdir dir_name));
    SHOW_LINE([cd], Cd a directory.(cd dir_name));
    SHOW_LINE([ls], List a directory.(ls));
    SHOW_LINE([pwd], Print path info.(pwd));
    SHOW_LINE([creat], Create a file.(creat file_name));
    SHOW_LINE([rm], Remove a file.(rm file_name));
    // SHOW_LINE([save], Save a file tree.());
    // SHOW_LINE([reload], Reload a file tree.);
    SHOW_LINE([vim], Eidt a file.(vim file_name));
    SHOW_LINE([cat], Show a file.(cat file_name));
    SHOW_LINE([upload], Upload a file.(upload file_name));
    SHOW_LINE([download], Download a file.(download file_name target_name));
    SHOW_LINE([quit], Quit.(quit));
    SHOW_LINE([tree], Display file tree.(tree));
    SHOW_LINE([menu], Help.(menu));
#undef SHOW_LINE
    return 0;
}

int cmd_quit(const int *argc, char **argv) {
    file_system->disk->writeBack();
    if (*argc == 0) return -1;
    return -2;
}

void get_cmd(const char *buffer, char *cmd) {
    int f_space_index = str_find(buffer, ' ');
    f_space_index = (f_space_index == -1) ? strlen(buffer) - 1 : f_space_index;
    str_cpy_s(cmd, f_space_index, buffer);
}

int get_cmd_index(char *cmd) {
    int i;
    for (i = 0; i < cmds_len; i++) {
        if (strcmp(fs_cmds[i], cmd) == 0) {
            return i;
        }
    }
    return -1;
}

int cmd_vim(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *arg_path = argv[1];
    char base_path[64];
    char file_path[64];
    split_path(arg_path, base_path, file_path);
    int flag = fs_vim(base_path, file_path);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("File name doesn't exists!\n\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_cat(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *arg_path = argv[1];
    char base_path[64];
    char file_path[64];
    split_path(arg_path, base_path, file_path);
    int flag = fs_cat(base_path, file_path);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("File name doesn't exists!\n\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_upload(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *file_name = argv[1];
    int flag = fs_upload(file_name);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("File name exists!\n\n");
            break;
        case 3:
            printf("Open file failed!\n\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_download(const int *argc, char **argv) {
    if (*argc == 0) return -1;
    char *file_name = argv[1];
    char *output_path = argv[2];
    int flag = fs_download(file_name, output_path);
    switch (flag) {
        case -1:
            printf("Error path!\n");
            break;
        case 1:
            printf("File name doesn't exists!\n\n");
            break;
        default:
            break;
    }
    return flag;
}

int cmd_tree(const int *argc, char **argv) {
    printf("%s\n", file_system->cur_node->file_name);
    fs_tree(file_system->cur_node, 0);
    return 0;
}

int cmd_info(const int *argc, char **argv) {
    if (*argc == 1) {
        printf("Disk Size: %d B\n", file_system->disk->diskTotalSize);
        printf("Total disk block: %d\n", file_system->disk->blockTotalCnt);
        printf("Remaining disk block: %d\n", file_system->disk->emptyCnt);
    } else {
        char *arg_path = argv[1];
        char base_path[64];
        char file_path[64];
        split_path(arg_path, base_path, file_path);
        // int flag = fs_cat(base_path, file_path);
        FileTreeNode *path_node = fs_loc_node(base_path);
        if (path_node == nullptr) return -1;
        FileTreeNode *p = path_node->child;
        while (p != nullptr) {
            if (strcmp(p->file_name, file_path) == 0) goto found;
            p = p->sibling;
        }
        return 1;
        found:
        IndexItem item = file_system->disk->indexItems[p->index_i];
        printf("File Name: %s\n", p->file_name);
        printf("File Size: %d B\n", item.fileLength);
        printf("File Type: %s\n", item.fileType == 'f' ? "File" : "Directory");
        return 0;
    }
    return 0;
}
