#include "verify.h"
#include <stdio.h>
#include <string.h>


static void printDeleted(Node *node)
{
    if (node->isDirectory)
    {
        if (node->childCount == 0)
        {
            printf(
                "Deleted: %s\n",
                node->path
            );

            return;
        }

        for (int i = 0; i < node->childCount; i++)
        {
            printDeleted(node->children[i]);
        }
    }
    else
    {
        printf(
            "Deleted: %s\n",
            node->path
        );
    }
}


static void printAdded(Node *node)
{
    if (node->isDirectory)
    {
        if (node->childCount == 0)
        {
            printf(
                "Added: %s\n",
                node->path
            );

            return;
        }

        for (int i = 0; i < node->childCount; i++)
        {
            printAdded(node->children[i]);
        }
    }
    else
    {
        printf(
            "Added: %s\n",
            node->path
        );
    }
}


static void compareNodes(Node *oldNode, Node *newNode)
{
    if (strcmp(oldNode->hash, newNode->hash) == 0)
    {
        return;
    }

    if (!oldNode->isDirectory &&
        !newNode->isDirectory)
    {
        printf(
            "Modified: %s\n",
            newNode->path
        );

        return;
    }

    if (oldNode->isDirectory !=
        newNode->isDirectory)
    {
        printDeleted(oldNode);
        printAdded(newNode);

        return;
    }

    int i = 0;
    int j = 0;

    while (i < oldNode->childCount &&
           j < newNode->childCount)
    {
        Node *oldChild =
            oldNode->children[i];

        Node *newChild =
            newNode->children[j];

        int result = strcmp(
            oldChild->path,
            newChild->path
        );

        if (result == 0)
        {
            compareNodes(
                oldChild,
                newChild
            );

            i++;
            j++;
        }
        else if (result < 0)
        {
            printDeleted(oldChild);
            i++;
        }
        else
        {
            printAdded(newChild);
            j++;
        }
    }

    while (i < oldNode->childCount)
    {
        printDeleted(
            oldNode->children[i]
        );

        i++;
    }

    while (j < newNode->childCount)
    {
        printAdded(
            newNode->children[j]
        );

        j++;
    }
}


int verifyTree(Node *oldRoot, Node *root)
{
    if (strcmp(oldRoot->hash, root->hash) == 0)
    {
        printf(
            "File tracker verification successful\n"
        );

        return 0;
    }

    printf(
        "File tracker verification failed\n"
    );

    printf("\nChanges:\n");

    compareNodes(
        oldRoot,
        root
    );

    return 1;
}