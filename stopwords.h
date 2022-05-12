#ifndef STOPWORDS_H
#define STOPWORDS_H

#define STR_MAXSIZE 256

#include "customstring.h"
#include "keyreg.h"
#include "btree.h"

int insertStopWords(char* filename);
void printStopWords();
int isStopWord(char* sw);

#endif // STOPWORDS_H
