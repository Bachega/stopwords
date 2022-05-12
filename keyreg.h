#ifndef KEYREG_H
#define KEYREG_H

#define STR_MAXSIZE 256

#include "customlist.h"

typedef struct keyreg{
    char str[STR_MAXSIZE];
    int fpos;
}KeyReg;

#endif // KEYREG_H
