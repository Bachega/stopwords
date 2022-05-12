#include "stopwords.h"

static char stopwordsfile[] = "stopwords.bin";

int insertStopWords(char* filename) {
    FILE* file = fopen(filename, "r");
    char* str = NULL;
    TreeHeader* header = NULL;
    KeyReg k; k.str[0] = '\0'; k.fpos = -1;

    if(file == NULL) {
        printf("Nao foi possivel inserir as stopwords");
        return 0;
    }

    str = (char*)malloc(sizeof(char)*STR_MAXSIZE); *str = '\0';
    header = generateTreeFile(stopwordsfile);
    free(header);

    while(!feof(file)) {
        loadStr(&str, file);
        strcpy(k.str, str);
        insertKey(stopwordsfile, k);
        k.str[0] = '\0';
        str[0] = '\0';
    }

    free(str);
    return 1;
}

void printStopWords() {
    printf("[STOPWORDS]\n\n");
    printBTreeInOrder(stopwordsfile);
}

int isStopWord(char* sw) {
    KeyReg k;

    if(isValidWord(sw)) {
        strcpy(k.str, sw); k.fpos = -1;
        k = fetchKey(stopwordsfile, k);
        if(k.str[0] == '\0') return 0;
        else return 1;
    } else {
        return 1;
    }
}
