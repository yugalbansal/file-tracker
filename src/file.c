#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

static void merge(Node **arr, int left, int mid, int right)
{
    int leftSize = mid - left + 1;
    int rightSize = right - mid;
    Node **leftArr = malloc(leftSize * sizeof(Node *));
    Node **rightArr = malloc(rightSize * sizeof(Node *));

    for (int i = 0; i < leftSize; i++) leftArr[i] = arr[left + i];
    for (int i = 0; i < rightSize; i++) rightArr[i] = arr[mid + 1 + i];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < leftSize && j < rightSize)
    {
        if (strcmp(leftArr[i]->path, rightArr[j]->path) <= 0)
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < leftSize)
    {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < rightSize)
    {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

static void mergeSort(Node **arr, int left, int right)
{
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

Node *createNode(char *name, int isDirectory)
{
    Node *node = malloc(sizeof(Node));
    if (node == NULL) return NULL;
    strcpy(node->path, name);
    node->isDirectory = isDirectory;
    node->children = NULL;
    node->childCount = 0;
    node->capacity = 0;
    return node;
}

void addChild(Node *parent, Node *child)
{
    if (parent->childCount == parent->capacity)
    {
        if (parent->capacity == 0) parent->capacity = 4;
        else parent->capacity *= 2;
        parent->children = realloc(parent->children, parent->capacity * sizeof(Node *));
    }
    parent->children[parent->childCount] = child;
    parent->childCount++;
}

void sortChildren(Node *node)
{
    if (node == NULL || node->childCount <= 1) return;
    for (int i = 0; i < node->childCount; i++)
    {
        if (node->children[i]->isDirectory) sortChildren(node->children[i]);
    }
    mergeSort(node->children, 0, node->childCount - 1);
}

void freeNode(Node *node)
{
    if (node == NULL) return;
    for (int i = 0; i < node->childCount; i++) freeNode(node->children[i]);
    free(node->children);
    free(node);
}
