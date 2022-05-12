#include "reg.h"

static char regfile[] = "reg.bin";

RegHeader* generateRegFile() {
    FILE* file = fopen(regfile, "wb");
    RegHeader* header = NULL;

    if(file==NULL) {
        printf("Nao foi possivel gerar o arquivo dos registros\n");
        return NULL;
    }

    header = (RegHeader*)malloc(sizeof(struct regheader));
    header->top = 0;

    fwrite(header, sizeof(struct regheader), 1, file);
    fclose(file);

    return header;
}

RegHeader* loadRegHeader() {
    FILE* file = fopen(regfile, "rb");
    RegHeader* header = NULL;

    if(file==NULL) {
        return generateRegFile();
    }

    header = (RegHeader*)malloc(sizeof(struct regheader));
    fread(header, sizeof(RegHeader), 1, file);
    fclose(file);

    return header;
}

RegNode* createRegNode() {
    RegNode* r = (RegNode*)malloc(sizeof(RegNode));
    r->filepath[0] = '\0';
    r->arq = 0;
    r->num = 0;
    r->next = -1;
    return r;
}

int insertRegNode(Avl* a, char* filepath, int i) {
    RegHeader* header = loadRegHeader();
    FILE* file = NULL;
    int pos = header->top;
    RegNode* node = createRegNode();

    strcpy(node->filepath, filepath);
    node->arq = i;
    node->num = a->num;

    file = fopen(regfile, "r+b");
    if(file == NULL) {
        printf("Nao foi possivel inserir o registro do node avl\n");
        free(node);
        return -1;
    }

    fseek(file, sizeof(RegHeader) + sizeof(RegNode)*header->top, SEEK_SET);
    fwrite(node, sizeof(RegNode), 1, file);
    header->top++;
    rewind(file);
    fwrite(header, sizeof(RegHeader), 1, file);
    fclose(file);

    free(header);
    free(node);
    return pos;
}

void enlistInReg(int start, int endd) {
    RegNode* node = NULL;
    FILE* file = NULL;

    file = fopen(regfile, "r+b");
    if(file == NULL) {
        printf("Nao foi possivel inserir o registro do node avl\n");
        return;
    }
    node = createRegNode();

    fseek(file, sizeof(RegHeader) + sizeof(RegNode)*start, SEEK_SET);
    fread(node, sizeof(RegNode), 1, file);
    while(node->next != (-1)) {
        fseek(file, sizeof(RegHeader) + sizeof(RegNode)*node->next, SEEK_SET);
        fread(node, sizeof(RegNode), 1, file);
    }
    fseek(file, sizeof(RegNode)*(-1), SEEK_CUR);
    node->next = endd;
    fwrite(node, sizeof(RegNode), 1, file);
    fclose(file);

    free(node);
}

void printRegFields(int pos) {
    RegNode* node = NULL;
    FILE* file = NULL;
    int nodePos = pos;

    file = fopen(regfile, "r+b");
    if(file == NULL) {
        printf("Nao foi possivel inserir o registro do node avl\n");
        return;
    }
    node = createRegNode();

    fseek(file, sizeof(RegHeader) + sizeof(RegNode)*nodePos, SEEK_SET);
    fread(node, sizeof(RegNode), 1, file);
    while(nodePos != (-1)) {
        printf("(%d;%d) ", node->arq, node->num);
        nodePos = node->next;
        if(nodePos!=(-1)) {
            fseek(file, sizeof(RegHeader) + sizeof(RegNode)*nodePos, SEEK_SET);
            fread(node, sizeof(RegNode), 1, file);
        }
    }

    fclose(file);
    free(node);
}

void enlistSearch(CustomList* l, int pos) {
    RegNode* node = NULL;
    FILE* file = NULL;
    int nodePos = pos;

    file = fopen(regfile, "r+b");
    if(file == NULL) {
        printf("Nao foi possivel realizar a pesquisa\n");
        return;
    }
    node = createRegNode();

    fseek(file, sizeof(RegHeader) + sizeof(RegNode)*nodePos, SEEK_SET);
    fread(node, sizeof(RegNode), 1, file);
    while(nodePos != (-1)) {
        insertListNode(l, node->arq, node->filepath);

        nodePos = node->next;
        if(nodePos!=-1) {
            fseek(file, sizeof(RegHeader) + sizeof(RegNode)*nodePos, SEEK_SET);
            fread(node, sizeof(RegNode), 1, file);
        }
    }

    fclose(file);
    free(node);
}
