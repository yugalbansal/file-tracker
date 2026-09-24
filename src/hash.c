#include "hash.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <openssl/sha.h>
#include <string.h>

int hashFile(char *path, char *hash)
{
    unsigned char raw[SHA256_DIGEST_LENGTH];
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror("cannot open the file");
        return -1;
    }
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    unsigned char buffer[4096];
    ssize_t bytes;
    while ((bytes = read(fd, buffer, sizeof(buffer))) > 0)
    {
        SHA256_Update(&ctx, buffer, bytes);
    }
    if (bytes == -1)
    {
        perror("read");
        close(fd);
        return -1;
    }
    SHA256_Final(raw, &ctx);
    close(fd);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(hash + i * 2, "%02x", raw[i]);
    }
    hash[64] = '\0';
    return 0;
}

void calculateDirectoryHash(Node *node)
{
    SHA256_CTX ctx;
    unsigned char raw[SHA256_DIGEST_LENGTH];
    SHA256_Init(&ctx);
    for (int i = 0; i < node->childCount; i++)
    {
        char data[1100];
        snprintf(data, sizeof(data), "%c %s %s", node->children[i]->isDirectory ? 'D' : 'F', node->children[i]->path, node->children[i]->hash);
        SHA256_Update(&ctx, data, strlen(data));
    }
    SHA256_Final(raw, &ctx);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(node->hash + i * 2, "%02x", raw[i]);
    }
    node->hash[64] = '\0';
}

void calculateTreeHash(Node *node)
{
    if (node == NULL) return;
    if (!node->isDirectory) return;
    for (int i = 0; i < node->childCount; i++) calculateTreeHash(node->children[i]);
    calculateDirectoryHash(node);
}

int sameFile(Node *a, Node *b)
{
    if (a == NULL || b == NULL) return 0;
    if (a->isDirectory || b->isDirectory) return 0;
    if (a->size != b->size) return 0;
    if (a->mtime != b->mtime) return 0;
    if (a->ctime != b->ctime) return 0;
    return 1;
}

void updateHashes(Node *node, Node *oldNode)
{
    if (node == NULL) return;
    if (!node->isDirectory)
    {
        if (sameFile(node, oldNode))
        {
            strcpy(node->hash, oldNode->hash);
        }
        else
        {
            hashFile(node->path, node->hash);
        }
        return;
    }
    for (int i = 0; i < node->childCount; i++)
    {
        Node *oldChild = NULL;
        if (oldNode != NULL && oldNode->isDirectory)
        {
            for (int k = 0; k < oldNode->childCount; k++)
            {
                if (strcmp(oldNode->children[k]->path, node->children[i]->path) == 0)
                {
                    oldChild = oldNode->children[k];
                    break;
                }
            }
        }
        updateHashes(node->children[i], oldChild);
    }
    calculateDirectoryHash(node);
}
