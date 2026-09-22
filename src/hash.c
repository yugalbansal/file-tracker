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
        snprintf(
            data,
            sizeof(data),
            "%c %s %s",
            node->children[i]->isDirectory ? 'D' : 'F',
            node->children[i]->path,
            node->children[i]->hash
        );
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

    for (int i = 0; i < node->childCount; i++)
    {
        calculateTreeHash(node->children[i]);
    }
    calculateDirectoryHash(node);
}

void updateHashes(Node *node)
{
    if (node == NULL) return;
    if (!node->isDirectory)
    {
        hashFile(node->path, node->hash);
        return;
    }
    for (int i = 0; i < node->childCount; i++)
    {
        updateHashes(node->children[i]);
    }
    calculateDirectoryHash(node);
}
