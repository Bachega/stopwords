#include "app.h"

int start(int argc, char* argv[]) {
    RegHeader* hR = NULL;
    char* str = NULL;

    if(argc==3) {
        hR = generateRegFile();
        free(hR);
        str = (char*)malloc(sizeof(char)*STR_MAXSIZE); *str = '\0';
        strcpy(str, argv[2]);
        insertStopWords(str);
        strcpy(str, argv[1]);
        insertWords(str);
        free(str);
        return 1;
    }

    return 0;
}

void app(int argc, char* argv[]) {
    char search[(STR_MAXSIZE*10)];
    int x = 1;

    if(start(argc, argv)) {
        while(x!=0) {
            system("cls");
            printf("MENU\n\n");
            printf("1- Imprimir lista de stop words\n");
            printf("2- Imprimir indice\n");
            printf("3- Consultar\n");
            printf("0- Sair\n");
            scanf("%d", &x); fflush(stdin);
            system("cls");
            switch(x) {
                case 1:
                    printStopWords();
                    printf("\n");
                    system("pause");
                break;
                case 2:
                    printWords();
                    printf("\n");
                    system("pause");
                break;
                case 3:
                    printf("Digite as palavras que deseja consultar:\n");
                    fgets(search, (STR_MAXSIZE*10), stdin); fflush(stdin);
                    strtok(search, "\n");
                    printf("\n");
                    searchWords(search);
                    printf("\n");
                    system("pause");
                break;
            }
        }
    }
}
