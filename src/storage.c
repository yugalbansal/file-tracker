#include "storage.h"
#include <stdio.h>
#include <sys/stat.h>

int createFt(char *path)
{
    char createPath[1000];
    snprintf(createPath, sizeof(createPath), "%s/.ft", path);
    return mkdir(createPath, 0755);
}
