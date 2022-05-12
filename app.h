#ifndef APP_H
#define APP_H

#define STR_MAXSIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avltree.h"
#include "btree.h"
#include "customlist.h"
#include "keyreg.h"
#include "reg.h"
#include "stopwords.h"
#include "words.h"

int start(int argc, char* argv[]);
void app(int argc, char* argv[]);

#endif // APP_H
