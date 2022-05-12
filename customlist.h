#ifndef CUSTOMLIST_H
#define CUSTOMLIST_H

#define STR_MAXSIZE 256

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct listnode{
    char filepath[STR_MAXSIZE];
    int arq;
    int num;
	struct listnode* next;
}ListNode;

typedef struct customlist{
    struct listnode* head;
    struct listnode* tail;
}CustomList;

ListNode* createListNode();
CustomList* createList();
int isEmptyList(CustomList* l);
void insertListNode(CustomList* l, int arq, char* filepath);
void deleteList(CustomList** l);
int printListByNum(CustomList* l, int num);

#endif // CUSTOMLIST_H
