#ifndef AVLTREE_H
#define AVLTREE_H

#define STR_MAXSIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct avl{
	char info[STR_MAXSIZE];
    int num;
	int bf;
	struct avl* left;
	struct avl* right;
}Avl;

Avl* rotateLeft(Avl* p);
Avl* rotateRight(Avl* p);
Avl* rotateRightLeft(Avl* p);
Avl* rotateLeftRight(Avl* p);
int insertAvlNode(Avl** a, char* info);
void printAvlTreeInOrder(Avl* t);
void deleteAvlTree(Avl** a);

#endif // AVLTREE_H
