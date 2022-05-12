#include "avltree.h"

Avl* rotateLeft(Avl* p) {
	Avl* u = p->right;
	Avl* t2 = u->left;
	u->left = p;
	p->right = t2;
	return u;
}

Avl* rotateRight(Avl* p) {
	Avl* u = p->left;
	Avl* t2 = u->right;
	u->right = p;
	p->left = t2;
	return u;
}

Avl* rotateRightLeft(Avl* p) {
	Avl* u = p->right;
	Avl* v = u->left;
	Avl* t2 = v->left;
	Avl* t3 = v->right;
	p->right = t2;
	u->left = t3;
	v->left = p;
	v->right = u;
	return v;
}

Avl* rotateLeftRight(Avl* p) {
	Avl* u = p->left;
	Avl* v = u->right;
	Avl* t2 = v->right;
	Avl* t3 = v->left;
	p->left = t2;
	u->right = t3;
	v->right = p;
	v->left = u;
	return v;
}

int insertAvlNode(Avl** a, char* info) {
	if(*a==NULL) {
		*a = (Avl*)malloc(sizeof(Avl));
        strcpy((*a)->info, info);
        (*a)->num = 1;
		(*a)->bf = 0;
		(*a)->left = NULL;
		(*a)->right = NULL;
		return 1;
	}

	if(strcmp(info, (*a)->info ) < 0) {
		if(insertAvlNode(&((*a)->left), info)) {
			switch((*a)->bf) {
				case -1:
					(*a)->bf = 0;
				return 0;
				case 0:
					(*a)->bf = 1;
				return 1;
				case 1:
					if((*a)->left->bf >= 0) {
						*a = rotateRight(*a);
						(*a)->right->bf = 0;
					} else {
						*a = rotateLeftRight(*a);
						switch((*a)->bf) {
							case -1:
								(*a)->left->bf = 1;
								(*a)->right->bf = 0;
							break;
							case 0:
								(*a)->left->bf = 0;
								(*a)->right->bf = 0;
							break;
							case 1:
								(*a)->left->bf = 0;
								(*a)->right->bf = -1;
							break;
						}
					}
					(*a)->bf = 0;
				return 0;
			}
		}
	} else if(strcmp(info, (*a)->info ) > 0) {
        if(insertAvlNode(&(*a)->right, info)) {
            switch((*a)->bf) {
                case 1:
                    (*a)->bf = 0;
                return 0;
                case 0:
                    (*a)->bf = -1;
                return 1;
                case -1:
                    if((*a)->right->bf <= 0) {
                        *a = rotateLeft(*a);
                        (*a)->left->bf = 0;
                    } else {
                        *a = rotateRightLeft(*a);
                        switch((*a)->bf) {
                            case -1:
                                (*a)->left->bf = 1;
                                (*a)->right->bf = 0;
                            break;
                            case 0:
                                (*a)->left->bf = 0;
                                (*a)->right->bf = 0;
                            break;
                            case 1:
                                (*a)->left->bf = 0;
                                (*a)->right->bf = -1;
                            break;
                        }
                    }
                    (*a)->bf = 0;
                return 0;
            }
        }
	} else {
        (*a)->num++;
        return 0;
	}
}

void printAvlTreeInOrder(Avl* t) {
	if(t!=NULL) {
        printAvlTreeInOrder(t->left);
		printf("%s || %d || %d\n", t->info, t->num, t->bf);
		printAvlTreeInOrder(t->right);
	}
}

void deleteAvlTree(Avl** a) {
    if((*a)!=NULL) {
        deleteAvlTree(&((*a)->left));
        deleteAvlTree(&((*a)->right));
        free((*a));
        *a = NULL;
    }
}
