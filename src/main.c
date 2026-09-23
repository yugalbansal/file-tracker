#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "storage.h"
#include "scanner.h"
#include "file.h"
#include "hash.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Error: Usage: ./merkletree <command> <directory>\n");
        return 1;
    }

    char path[1000];
    strcpy(path, argv[2]);

    char indexPath[1100];
    snprintf(indexPath, sizeof(indexPath), "%s/.ft/index", path);

    if (strcmp(argv[1], "init") == 0)
    {
        if (access(indexPath, F_OK) == 0)
        {
            printf("file tracker already initialized\n");
            return 1;
        }

        if (createFt(path) != 0)
        {
            printf("Error: could not create .ft directory\n");
            return 1;
        }
        
        Node *root = ReadDirectory(path);
        if (root == NULL)
        {
            printf("Error: could not read directory\n");
            return 1;
        }
        
        updateHashes(root);

        int fd = open(indexPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            printf("Error: could not create index\n");
            freeNode(root);
            return 1;
        }
        saveTree(fd, root);
        close(fd);

        freeNode(root);
    }
    else
    {
        printf("Error: unknown command\n");
        printf("Usage: ./merkletree <command> <directory>\n");
        return 1;
    }

    return 0;
}
