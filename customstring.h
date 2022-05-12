#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void loadStr(char** str, FILE* file);
void loadStrWord(char** str, FILE* file);
//int isValidChar(char c);
int isValidWord(char* str);

#endif // CUSTOMSTRING_H
