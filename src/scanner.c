#define _DEFAULT_SOURCE

#include "scanner.h"
#include "file.h"

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

Node *ReadDirectory(char *path)
{
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        printf("Could not open directory\n");
        return NULL;
    }

    Node *root = createNode(path, "", 1);

    if (root == NULL)
    {
        closedir(dir);
        return NULL;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0 ||
            strcmp(entry->d_name, ".ft") == 0)
        {
            continue;
        }

        char newPath[1000];

        snprintf(
            newPath,
            sizeof(newPath),
            "%s/%s",
            path,
            entry->d_name
        );

        struct stat st;

        if (lstat(newPath, &st) == -1)
        {
            continue;
        }

        if (S_ISREG(st.st_mode))
        {
            Node *file = createNode(
                newPath,
                "",
                0
            );

            if (file != NULL)
            {
                file->size = st.st_size;

                file->mtime =
                    (long long)st.st_mtim.tv_sec * 1000000000LL
                    + st.st_mtim.tv_nsec;

                file->ctime =
                    (long long)st.st_ctim.tv_sec * 1000000000LL
                    + st.st_ctim.tv_nsec;

                addChild(root, file);
            }
        }

        else if (S_ISDIR(st.st_mode))
        {
            Node *directory = ReadDirectory(newPath);

            if (directory != NULL)
            {
                addChild(root, directory);
            }
        }
    }

    sortChildren(root);

    closedir(dir);

    return root;
}