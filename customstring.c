#include "customstring.h"

void loadStr(char** str, FILE* file) {
    int i=0;
    char c = '\0';
    while((c=fgetc(file))!='\n' && (c!=EOF)) {
        *((*str)+i) = c;
        i++;
    }
    *((*str)+i) = '\0';
    if(*(*str)=='\n') *((*str)) = '\0';
}

void loadStrWord(char** str, FILE* file) {
    int i=0;
    char c = '\0';
    while(((c=fgetc(file))!=' ') && (c!='.') && (c!=',') && (c!='!') && (c!='?') && (c!=';') && (c!=':') && (c!='\n') && (c!=EOF)) {
        *((*str)+i) = c;
        i++;
    }
    *((*str)+i) = '\0';
    if(*(*str)=='\n') *((*str)) = '\0';
}

int isValidWord(char* str) {
    int i=0;
    if(strlen(str)<=1 || str[1] == '\0') return 0;

    for(i=0; str[i]; i++)
        if(str[i]!='-' && (str[i] < 'A' || str[i] > 'z')) return 0;

    return 1;
}
