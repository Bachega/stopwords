#ifndef BTREE_H
#define BTREE_H

#define ORDER 5

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "keyreg.h"
#include "reg.h"

typedef struct treenode{
    int numKeys;
    struct keyreg key[ORDER]; //vetor de chaves
    int son[ORDER+1];
}TreeNode;

typedef struct treeheader{
    int root;
    int top;
}TreeHeader;

TreeHeader* generateTreeFile();
TreeHeader* loadTreeHeader();
TreeNode* createBTreeNode();
TreeNode* split(TreeNode* a, KeyReg* med);
int insertKeyAux(TreeNode* node, TreeHeader* header, KeyReg key, int nodePos, FILE* file);
int insertKey(char* filename, KeyReg k);
KeyReg fetchKeyAux(TreeNode* root, FILE* file, int pos, KeyReg key);
KeyReg fetchKey(char* filename, KeyReg key);
void printBTreeInOrderAux(int pos, FILE* file);
void printBTreeInOrder(char* filename);

void printIndex(char* filename);
void printIndexAux(FILE* file, int root);

#endif // BTREE_H
