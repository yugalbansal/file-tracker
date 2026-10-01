#ifndef HASH_H
#define HASH_H

#include "file.h"

int hashFile(char *path, char *hash);

void calculateTreeHash(Node *node);
void calculateDirectoryHash(Node *node);

int sameFile(Node *a, Node *b);

void updateHashes(Node *node, Node *oldNode);

#endif