#include "verify.h"
#include <stdio.h>
#include <string.h>

int verifyTree(Node *oldRoot, Node *root)
{
    if (strcmp(oldRoot->hash, root->hash) == 0)
    {
        printf("File tracker verification successful\n");
        return 0;
    }

    printf("File tracker verification failed\n");
    return 1;
}
