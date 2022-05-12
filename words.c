#include "words.h"

static char wordsfile[] = "words.bin";

int insertWords(char* filename) {
    FILE* file = fopen(filename, "r");
    FILE* bfile = NULL;
    char* str = NULL;
    TreeHeader* header = NULL;
    int i = 1;

    if(file == NULL) {
        printf("Nao foi possivel abrir o arquvio de base de dados das palavras\n");
        return 0;
    }

    str = (char*)malloc(sizeof(char)*STR_MAXSIZE); *str = '\0';
    header = generateTreeFile(wordsfile);
    free(header);

    while(!feof(file)) {
        loadStr(&str, file);
        bfile = fopen(str, "r");
        if(bfile == NULL)
            printf("Nao foi possivel inserir as palavras do arquivo: %s\n", str);
        else {
            insertWordsInBTree(&bfile, str, i);
        }
        str[0] = '\0';
        i++;
    }

    free(str);
    return 1;
}

void insertWordsInBTree(FILE** file, char* filepath, int i) {
    char* str = (char*)malloc(sizeof(char)*STR_MAXSIZE); *str = '\0';
    Avl* a = NULL;

    while(!feof(*file)) {
        loadStrWord(&str, *file);
        if(str[0]!='\0')
            if(!isStopWord(str))
                insertAvlNode(&a, str);
        str[0] = '\0';
    }
    fclose(*file); *file = NULL;

    insertBTreeFromAvl(a, filepath, i);

    free(str);
    deleteAvlTree(&a);
}

void insertBTreeFromAvl(Avl* a, char* filepath, int i) {
    int pos = -1;
    KeyReg k; k.fpos = -1;
    if(a!=NULL) {
        insertBTreeFromAvl(a->left, filepath, i);

        pos = insertRegNode(a, filepath, i);
        if(pos!=(-1)) {
            strcpy(k.str, a->info);
            k.fpos = pos;
            if(!insertKey(wordsfile, k)) {
                k.fpos = -1;
                k = fetchKey(wordsfile, k);
                enlistInReg(k.fpos, pos);
            }
        }

        insertBTreeFromAvl(a->right, filepath, i);
    }
}

void printWords() {
    printf("[PALAVRA] | (ARQ;QNTD)\n\n");
    printBTreeInOrder(wordsfile);
}

void searchWords(char* str) {
    char* temp = (char*)malloc(sizeof(char)*STR_MAXSIZE); *temp = '\0';
    CustomList* l = createList();
    int j = 0;
    int cont = 0;
    int x = 1;
    int t = 0;
    int i = 0;
    KeyReg k;

    for(i=0; x!=0; i++) {
        if(str[i]!=' ' && str[i]!='\0') {
            *(temp+j) = str[i];
            j++;
        } else {
            cont++;
            *(temp+j) = '\0';
            j = 0;
            strcpy(k.str, temp); k.fpos = -1;
            k = fetchKey(wordsfile, k);

            if(k.fpos!=(-1))
                enlistSearch(l, k.fpos);

            if(str[i]=='\0')
                x=0;
        }
    }

    printf("Documentos:\n");
    t = printListByNum(l, cont);
    printf("\nQuantidade: %d\n", t);

    free(temp);
    deleteList(&l);
}
