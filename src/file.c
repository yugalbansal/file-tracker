#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

Node *createNode(char *name, int isDirectory)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return NULL;
    strcpy(node->path, name);
    node->isDirectory = isDirectory;
    node->children = NULL;
    node->childCount = 0;
    node->capacity = 0;
    return node;
}

void addChild(Node *parent, Node *child)
{
    if (parent->childCount == parent->capacity)
    {
        if (parent->capacity == 0) parent->capacity = 4;
        else parent->capacity *= 2;
        parent->children = realloc(parent->children, parent->capacity * sizeof(Node *));
    }
    parent->children[parent->childCount] = child;
    parent->childCount++;
}

void freeNode(Node *node)
{
    if (node == NULL) return;
    for (int i = 0; i < node->childCount; i++) freeNode(node->children[i]);
    free(node->children);
    free(node);
}
