#include "ftree/ftn.h"

FileTreeNode *ftn_new(const char *file_name, char file_type, FileTreeNode *parent) {
    FileTreeNode *ftn = (FileTreeNode *) kmalloc(sizeof(FileTreeNode));
    strcpy(ftn->file_name, file_name);
    ftn->file_type = file_type;
    ftn->child = ftn->sibling = NULL;
    ftn->parent = parent;
    ftn->index_i = 0;
    return ftn;
}

void ftn_add_node(FileTreeNode *cur, FileTreeNode *new_node) {
    if (cur->child == NULL) {
        cur->child = new_node;
        new_node->parent = cur;
    } else {
        FileTreeNode *p = cur->child;
        while (p->sibling != NULL) p = p->sibling;
        p->sibling = new_node;
        new_node->parent = p;
    }
}

void ftn_free_node(FileTreeNode *cur) {
    if (cur == NULL) return;
    ftn_free_node(cur->child);
    ftn_free_node(cur->sibling);
    kfree(cur);
}

void ftn_del_node(FileTreeNode *cur) {
    if (cur == NULL) return;
    if (cur->parent->child == cur) {
        cur->parent->child = cur->sibling;
    } else if (cur->parent->sibling == cur) {
        cur->parent->sibling = cur->sibling;
    } else return;

    if (cur->sibling) cur->sibling->parent = cur->parent;

    ftn_free_node(cur->child);
    kfree(cur);
}

void ftn_traverse(FileTreeNode *root) {
    if (root == NULL) return;
    printf("%s\n", root->file_name);
    ftn_traverse(root->child);
    ftn_traverse(root->sibling);
}

void ftn_rename(FileTreeNode *cur, const char *name) {
    if (cur == NULL) return;
    strcpy(cur->file_name, name);
}

FileTreeNode *ftn_father(FileTreeNode *cur) {
    FileTreeNode *p = cur;
    while (p->parent != p && p->parent->child != p) p = p->parent;
    return p->parent;
}