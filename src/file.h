#ifndef FILE_H
#define FILE_H

typedef struct Node{
    char path[1000];
    char hash[65];
    int isDirectory;

    struct Node **children;
    int childCount;
    int capacity;
} Node;

Node *createNode(char *name, char *hash, int isDirectory);
void addChild(Node *parent, Node *child);
void sortChildren(Node *node);
void freeNode(Node *node);

#endif
