#ifndef REG_H
#define REG_H

#define STR_MAXSIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customlist.h"
#include "avltree.h"
#include "btree.h"

typedef struct regheader{
    int top;
}RegHeader;

typedef struct regnode{
    char filepath[STR_MAXSIZE];;
    int arq;
    int num;
    int next;
}RegNode;

RegHeader* generateRegFile();
RegHeader* loadRegHeader();
RegNode* createRegNode();
int insertRegNode(Avl* a, char* filepath, int i);
void enlistInReg(int start, int endd);
void printRegFields(int pos);
void enlistSearch(CustomList* l, int pos);

#endif // REG_H
