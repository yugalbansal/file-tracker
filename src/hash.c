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
}
