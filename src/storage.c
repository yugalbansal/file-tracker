#include "storage.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>


typedef struct
{
    int fd;
    char buffer[4096];
    int pos;
    int size;
} Reader;


static int readLine(Reader *reader, char *line)
{
    int i = 0;

    while (1)
    {
        if (reader->pos >= reader->size)
        {
            reader->size = read(
                reader->fd,
                reader->buffer,
                sizeof(reader->buffer)
            );

            reader->pos = 0;

            if (reader->size <= 0)
            {
                if (i == 0)
                {
                    return 0;
                }

                break;
            }
        }

        char c = reader->buffer[reader->pos];

        reader->pos++;

        if (c == '\n')
        {
            break;
        }

        line[i] = c;
        i++;
    }

    line[i] = '\0';

    return 1;
}


static int isChildPath(char *parent, char *child)
{
    int len = strlen(parent);

    if (strcmp(parent, "/") == 0)
    {
        return child[0] == '/';
    }

    if (strncmp(parent, child, len) != 0)
    {
        return 0;
    }

    if (child[len] == '/')
    {
        return 1;
    }

    return 0;
}


int createFt(char *path)
{
    char createPath[1000];

    snprintf(
        createPath,
        sizeof(createPath),
        "%s/.ft",
        path
    );

    return mkdir(createPath, 0755);
}


int saveTree(int fd, Node *node)
{
    char buffer[1400];

    char type;

    int n;

    if (node->isDirectory)
    {
        type = 'D';

        n = snprintf(
            buffer,
            sizeof(buffer),
            "%c\t%s\t%s\n",
            type,
            node->path,
            node->hash
        );
    }
    else
    {
        type = 'F';

        n = snprintf(
            buffer,
            sizeof(buffer),
            "%c\t%s\t%s\t%lld\t%lld\t%lld\n",
            type,
            node->path,
            node->hash,
            node->size,
            node->mtime,
            node->ctime
        );
    }

    if (write(fd, buffer, n) != n)
    {
        return -1;
    }

    for (int i = 0; i < node->childCount; i++)
    {
        if (saveTree(fd, node->children[i]) == -1)
        {
            return -1;
        }
    }

    return 0;
}


Node *loadTree(int fd)
{
    Reader reader;

    reader.fd = fd;
    reader.pos = 0;
    reader.size = 0;

    char line[1400];

    Node *root = NULL;

    Node **stack = NULL;
    int stackSize = 0;

    while (readLine(&reader, line))
    {
        char type;
        char path[1000];
        char hash[65];

        long long size = 0;
        long long mtime = 0;
        long long ctime = 0;

        int count;

        if (line[0] == 'F')
        {
            count = sscanf(
                line,
                "%c\t%999[^\t]\t%64s\t%lld\t%lld\t%lld",
                &type,
                path,
                hash,
                &size,
                &mtime,
                &ctime
            );

            if (count != 6)
            {
                continue;
            }
        }
        else
        {
            count = sscanf(
                line,
                "%c\t%999[^\t]\t%64s",
                &type,
                path,
                hash
            );

            if (count != 3)
            {
                continue;
            }
        }

        Node *node = createNode(
            path,
            hash,
            type == 'D'
        );

        if (node == NULL)
        {
            continue;
        }

        if (type == 'F')
        {
            node->size = size;
            node->mtime = mtime;
            node->ctime = ctime;
        }

        if (root == NULL)
        {
            root = node;

            stack = malloc(sizeof(Node *));

            stack[0] = root;
            stackSize = 1;

            continue;
        }

        while (stackSize > 0 &&
               (!stack[stackSize - 1]->isDirectory ||
                !isChildPath(
                    stack[stackSize - 1]->path,
                    node->path
                )))
        {
            stackSize--;
        }

        if (stackSize == 0)
        {
            freeNode(node);
            break;
        }

        addChild(
            stack[stackSize - 1],
            node
        );

        if (node->isDirectory)
        {
            stack = realloc(
                stack,
                (stackSize + 1) * sizeof(Node *)
            );

            stack[stackSize] = node;
            stackSize++;
        }
    }

    free(stack);

    return root;
}