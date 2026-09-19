#ifndef FILE_H
#define FILE_H

typedef struct Node{
    char path[1000];
    int isDirectory;

    struct Node **children;
    int childCount;
    int capacity;
} Node;

Node *createNode(char *name, int isDirectory);
void addChild(Node *parent, Node *child);
void freeNode(Node *node);

#endif
