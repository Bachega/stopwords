#include "btree.h"

TreeHeader* generateTreeFile(char* filename) {
    FILE* file = fopen(filename, "wb");
    TreeHeader* header = NULL;

    if(file==NULL) {
        printf("Nao foi possivel gerar o arquivo da arvore\n");
        return NULL;
    }

    header = (TreeHeader*)malloc(sizeof(struct treeheader));
    header->root = -1;
    header->top = 0;

    fwrite(header, sizeof(struct treeheader), 1, file);
    fclose(file);

    return header;
}

TreeHeader* loadTreeHeader(char* filename) {
    FILE* file = fopen(filename, "rb");
    TreeHeader* header = NULL;

    if(file==NULL) {
        return generateTreeFile(filename);
    }

    header = (TreeHeader*)malloc(sizeof(struct treeheader));
    fread(header, sizeof(TreeHeader), 1, file);
    fclose(file);

    return header;
}

TreeNode* createBTreeNode() {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    node->numKeys = 0;
    for(i=0; i<(ORDER+1); i++) node->son[i] = -1;
    return node;
}

TreeNode* split(TreeNode* a, KeyReg* med) {
    TreeNode* b = createBTreeNode();
    int i, q = a->numKeys/2;
    b->numKeys = a->numKeys - q - 1;
    a->numKeys = q;
    strcpy(med->str, a->key[q].str); //chave mediana
    med->fpos = a->key[q].fpos;
    b->son[0] = a->son[q+1];

    for(i = 0; i < b->numKeys; i++) {
        b->key[i] = a->key[q+i+1];
        b->son[i+1] = a->son[q+i+2];
    }

    return b;
}

int insertKeyAux(TreeNode* node, TreeHeader* header, KeyReg key, int nodePos, FILE* file) {
    int pos, i, val = 0;
    TreeNode* tempNode = NULL;
    TreeNode* aux = NULL;
    KeyReg med;

    for(pos=0; pos<node->numKeys; pos++)
        if(strcmp(key.str, node->key[pos].str) == 0)
            return 0;
        else if(strcmp(key.str, node->key[pos].str) < 0)
            break;

    if(node->son[0] == -1) {
        for(i=node->numKeys; i>pos; i--) {
            node->key[i] = node->key[i-1];
            node->son[i+1] = node->son[i];
        }

        node->key[pos] = key;
        node->son[pos+1] = -1;
        node->numKeys++;

        if(node->numKeys != ORDER) {
            fseek(file, sizeof(TreeHeader) + sizeof(TreeNode)*nodePos, SEEK_SET);
            fwrite(node, sizeof(TreeNode), 1, file);
        }
        val = 1;
    } else {
        tempNode = createBTreeNode();

        fseek(file, (sizeof(TreeHeader)+sizeof(TreeNode)*node->son[pos]), SEEK_SET);
        fread(tempNode, sizeof(TreeNode), 1, file);

        val = insertKeyAux(tempNode, header, key, node->son[pos], file);

        if(tempNode->numKeys == ORDER) {
            aux = split(tempNode, &med);

            for(i=node->numKeys; i>pos; i--) {
                node->key[i] = node->key[i-1];
                node->son[i+1] = node->son[i];
            }

            node->key[pos] = med;
            node->son[pos+1] = header->top;
            node->numKeys++;

            fseek(file, (sizeof(TreeHeader)+sizeof(TreeNode)*node->son[pos]), SEEK_SET);
            fwrite(tempNode, sizeof(TreeNode), 1, file);

            fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->top), SEEK_SET);
            fwrite(aux, sizeof(TreeNode), 1, file);
            header->top++;

            rewind(file);
            fwrite(header, sizeof(TreeHeader), 1, file);
        }

        if(val) {
            fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*nodePos), SEEK_SET);
            fwrite(node, sizeof(TreeNode), 1, file);
        }
    }
    free(tempNode);
    free(aux);
    return val;
}

int insertKey(char* filename, KeyReg key) {
    FILE* file = NULL;
    TreeHeader* header = NULL;
    TreeNode* node = NULL;
    TreeNode* newRoot = NULL;
    TreeNode* tempNode = NULL;
    TreeNode* aux = NULL;
    KeyReg med;
    int ret = 0;

    if(key.str[0] == '\0') return 0;

    file = fopen(filename, "r+b");

    if(file==NULL) {
        printf("Nao foi possivel inserir a chave\n");
        return 0;
    }
    header = loadTreeHeader(filename);

    if(header->root == (-1)) {
        node = createBTreeNode();
        node->key[0] = key;
        node->numKeys = 1;

        header->root++;
        header->top++;
        fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->root), SEEK_SET);

        fwrite(node, sizeof(TreeNode), 1, file);
        rewind(file);
        fwrite(header, sizeof(TreeHeader), 1, file);
        ret = 1;
    } else {
        node = createBTreeNode();
        fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->root), SEEK_SET);
        fread(node, sizeof(TreeNode), 1, file);

        ret = insertKeyAux(node, header, key, header->root, file);

        if(node->numKeys == ORDER) {
            aux = split(node, &med);
            newRoot = createBTreeNode();
            newRoot->key[0] = med;
            newRoot->son[0] = header->root;
            newRoot->numKeys++;

            fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->root), SEEK_SET);
            fwrite(node, sizeof(TreeNode), 1, file);

            newRoot->son[1] = header->top;
            fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->top), SEEK_SET);
            fwrite(aux, sizeof(TreeNode), 1, file);
            header->top++;

            header->root = header->top;
            fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->root), SEEK_SET);
            fwrite(newRoot, sizeof(TreeNode), 1, file);
            header->top++;

            rewind(file);
            fwrite(header, sizeof(TreeHeader), 1, file);
        }
    }
    fclose(file);
    free(node);
    free(newRoot);
    free(tempNode);
    free(aux);
    free(header);
    return ret;
}

KeyReg fetchKeyAux(TreeNode* root, FILE* file, int pos, KeyReg key) {
    int i = 0;
    KeyReg k; k.str[0] = '\0'; k.fpos = -1;

    fseek(file, sizeof(TreeHeader) + sizeof(TreeNode)*root->son[pos], SEEK_SET);
    fread(root, sizeof(TreeNode), 1, file);

    for(i=0; i < root->numKeys; i++)
        if(strcmp(key.str, root->key[i].str) == 0) {
            free(root);
            return root->key[i];
        } else if(strcmp(key.str, root->key[i].str) < 0) break;

    if(root->son[i]!=(-1))
        k = fetchKeyAux(root, file, i, key);

    return k;
}

KeyReg fetchKey(char* filename, KeyReg key){
    int i = 0;
    KeyReg k; k.str[0] = '\0'; k.fpos = -1;
    TreeHeader* header = NULL;
    TreeNode* root = NULL;
    FILE* file = fopen(filename, "rb");

    if(file == NULL) return k;
    header = loadTreeHeader(filename);

    root = createBTreeNode();

    fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->root), SEEK_SET);
    fread(root, sizeof(TreeNode), 1, file);

    for(i=0; i < root->numKeys; i++)
        if(strcmp(key.str, root->key[i].str) == 0) {
            free(root);
            return root->key[i];
        } else if(strcmp(key.str, root->key[i].str) < 0) break;

    if(root->son[i]!=(-1))
        k = fetchKeyAux(root, file, i, key);

    fclose(file);
    free(root);
    free(header);

    return k;
}

void printBTreeInOrderAux(int pos, FILE* file) {
    if(pos==(-1)) return;
    TreeNode* node = NULL;
    int i = 0;

    node = (TreeNode*)malloc(sizeof(TreeNode));
    fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*pos), SEEK_SET);
    fread(node, sizeof(TreeNode), 1, file);

    for(i=0; i<node->numKeys; i++) {
        if(node->son[i]!=(-1)) printBTreeInOrderAux(node->son[i], file);

        printf("[%s]", node->key[i].str);
        if(node->key[i].fpos!=(-1)) {
            printf(" | ");
            printRegFields(node->key[i].fpos);
        }
        printf("\n");

    }

    if(node->son[i]!=-1) printBTreeInOrderAux(node->son[i], file);

    free(node);
}

void printBTreeInOrder(char* filename) {
    TreeNode* node = NULL;
    TreeHeader* header = NULL;
    int i = 0;

    FILE* file = fopen(filename, "rb");

    if(file == NULL) {
        printf("Nao foi possivel imprimir a arvore de forma crescente\n\n");
        system("pause");
        return;
    }
    header = loadTreeHeader(filename);


    node = createBTreeNode();
    fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*header->root), SEEK_SET);
    fread(node, sizeof(TreeNode), 1, file);

    for(i=0; i<node->numKeys; i++) {
        if(node->son[i]!=(-1)) printBTreeInOrderAux(node->son[i], file);

        printf("[%s]", node->key[i].str);
        if(node->key[i].fpos!=(-1)) {
            printf(" | ");
            printRegFields(node->key[i].fpos);
        }
        printf("\n");

    }
    if(node->son[i]!=(-1)) printBTreeInOrderAux(node->son[i], file);

    fclose(file);
    free(header);
    free(node);
}

void printIndexAux(FILE* file, int root) {
    TreeNode* node = createBTreeNode();
    int i;
    fseek(file, (sizeof(TreeHeader) + sizeof(TreeNode)*root), SEEK_SET);
    fread(node, sizeof(TreeNode), 1, file);

    printf("(%d) ", root);
    for(i=0; i<node->numKeys; i++)
        printf("{%d}[%s]", node->son[i], node->key[i].str);
    printf("{%d}", node->son[i]);
    printf("\n");
    for(i=0; i<=node->numKeys; i++) {
        if(node->son[i]==-1) break;
        printIndexAux(file, node->son[i]);
    }
    free(node);
}

void printIndex(char* filename) {
    TreeHeader* header = loadTreeHeader(filename);
    FILE* file = fopen(filename, "rb");
    int root = header->root;
    free(header);

    if(file==NULL) {
        printf("Nao foi possivel imprimir os indices da arvore\n");
        return;
    }

    printIndexAux(file, root);

    fclose(file);
}
