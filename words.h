#ifndef WORDS_H
#define WORDS_H

#define STR_MAXSIZE 256

#include "customstring.h"
#include "stopwords.h"
#include "keyreg.h"
#include "avltree.h"
#include "btree.h"
#include "reg.h"

int insertWords(char* filename);
void insertWordsInBTree(FILE** file, char* filepath, int i);
void insertBTreeFromAvl(Avl* a, char* filepath, int i);
void printWords();
void searchWords(char* str);


#endif // WORDS_H
