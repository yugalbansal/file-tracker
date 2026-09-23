#ifndef STORAGE_H
#define STORAGE_H

#include "file.h"

int createFt(char *path);

int saveTree(int fd, Node *node);
Node *loadTree(int fd);

#endif