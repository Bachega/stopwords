#include "customlist.h"

ListNode* createListNode() {
    ListNode* ln = (ListNode*)malloc(sizeof(ListNode));
    ln->filepath[0] = '\0';
    ln->arq = 0;
    ln->num = 0;
    ln->next = NULL;
    return ln;
}

CustomList* createList() {
    CustomList* l = (CustomList*)malloc(sizeof(CustomList));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

int isEmptyList(CustomList* l) {
    return ((l->head == NULL) && (l->tail == NULL));
}

void insertListNode(CustomList* l, int arq, char* filepath) {
    ListNode* temp = NULL;

    if(isEmptyList(l)) {
        l->head = createListNode();
        l->tail = l->head;
        strcpy(l->head->filepath, filepath);
        l->head->arq = arq;
        l->head->num++;
    } else {
        temp = l->head;

        while(temp!=NULL) {
            if(temp->arq == arq) {
                temp->num++;
                break;
            }
            temp = temp->next;
        }

        if(temp==NULL) {
            l->tail->next = createListNode();
            l->tail = l->tail->next;
            strcpy(l->tail->filepath, filepath);
            l->tail->arq = arq;
            l->tail->num++;
        }

        temp = NULL;
    }
}

void deleteList(CustomList** l) {
    ListNode* aux = NULL;
    ListNode* auxnext = NULL;

    if(!isEmptyList((*l))) {
        aux = (*l)->head;
        auxnext = aux->next;

        while(auxnext!=NULL) {
            aux->next = NULL;
            free(aux);
            aux = auxnext;
            auxnext = auxnext->next;
        }
        aux->next = NULL;
        free(aux);

        (*l)->head = (*l)->tail = NULL;
    }

    free((*l));
    (*l) = NULL;
}

int printListByNum(CustomList* l, int num) {
    ListNode* temp = l->head;
    int t = 0;
    while(temp!=NULL) {
        if(temp->num == num) {
            t++;
            printf("%s\n", temp->filepath);
        }
        temp = temp->next;
    }
    return t;
}
