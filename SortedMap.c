/**
 MIT License

 Copyright (c) 2018 Matias Barrientos.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "SortedMap.h"
#include <stdlib.h>
#include <assert.h>

typedef struct TreeNode TreeNode;

static TreeNode * _createTreeNode(const void * key, const void * data);

struct TreeNode {
    const void * key;
    /*! Puntero al dato */
    const void * data;

    /*! Puntero al siguiente nodo */
    TreeNode * next;

    /*! Puntero al anterior nodo */
    TreeNode * prev;
};

struct SortedMap {
    /*! Puntero al incio (cabeza) de la lista */
    TreeNode * head;

    /*! Puntero al final (cola) de la lista */
    TreeNode * tail;

    /*! Punteor para poder recorrer la lista */
    TreeNode * current;

    SortedMapCompareCallBack compare;

    /*! Cantidad de elemento en la lista */
    long count;
};

static TreeNode* _createTreeNode(const void * key, const void * data) {
    TreeNode  * new = (TreeNode *)malloc(sizeof(TreeNode));

    assert(new != NULL);

    new->key = key;
    new->data = data;
    new->prev = NULL;
    new->next = NULL;
    return new;
}


SortedMap * createSortedMap(SortedMapCompareCallBack compare) {
    SortedMap * new = (SortedMap *)malloc(sizeof(SortedMap));
    assert(new != NULL); // No hay memoria para reservar la SortedMapa.
    new->head = new->tail = new->current = NULL;
    new->count = 0;
    new->compare = compare;
    return new;
}

long sortedMapCount(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL) return 0;

    return list->count;
}

int emptySortedMap(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.
    return list->count == 0;
}

void * firstSortedMap(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL) return NULL;

    list->current = list->head;

    return (void *)list->current->data;
}

void * nextSortedMap(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL || list->current == NULL || list->current->next == NULL) return NULL;

    list->current = list->current->next;

    return (void *)list->current->data;
}

void * lastSortedMap(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL) return NULL;

    list->current = list->tail;

    return (void *)list->current->data;
}

void * prevSortedMap(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL || list->current == NULL || list->current->prev == NULL) return NULL;

    list->current = list->current->prev;

    return (void *)list->current->data;
}

void __pushFront(SortedMap * list, const void * key, const void * value) {
    assert(list != NULL); // list no puede ser NULL.

    TreeNode * new = _createTreeNode(key, value);

    if (list->head == NULL) {
        list->tail = new;
    } else {
        new->next = list->head;
        list->head->prev = new;
    }

    list->head = new;
    list->count += 1;
}


void insertSortedMap(SortedMap * list, const void * key, const void * value){

    assert(list != NULL); // list no puede ser NULL.

    TreeNode* aux= list->head;
    //the minimum element
    if (!aux || list->compare(aux->key,key)>0) {
        __pushFront (list, key, value);
        return;
    }


    while(aux->next && list->compare(aux->next->key,key)<0)
        aux=aux->next;

    list->current = aux;

    TreeNode* new = _createTreeNode(key, value);

    new->next = list->current->next;
    new->prev = list->current;

    if (list->current->next != NULL) {
        list->current->next->prev = new;
    }

    list->current->next = new;

    if (list->current == list->tail) {
        list->tail = new;
    }

    list->count += 1;
}

void * __popFront(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL) return NULL;

    TreeNode * aux = list->head;

    void * data = (void *)aux->data;

    if (list->head == list->tail) {
        list->tail = list->head = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    free(aux);

    list->count -= 1;

    return data;
}

void * __popBack(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    if (list->head == NULL) return NULL;

    TreeNode * aux = list->tail;

    void * data = (void *)aux->data;

    if (list->tail == list->head) {
        list->tail = list->head = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    free(aux);

    list->count -= 1;

    return data;
}

void * upperBoundSortedMap(SortedMap * map, const void * key){
    //printf("this function is not implemented yet\n");
    return NULL;
}

void* searchSortedMap(SortedMap * list, const void * key) {
    assert(list != NULL); // list no puede ser NULL.

    TreeNode* aux= list->head;
    //the minimum element
    while (aux && list->compare(aux->key,key)!=0 ) aux=aux->next;

    list->current=aux;
    if (list->head == NULL || list->current == NULL) return NULL;

    return (void *) aux->data;
}

void * eraseKeySortedMap(SortedMap * list, const void * key) {
    assert(list != NULL); // list no puede ser NULL.

    TreeNode* aux= list->head;
    //the minimum element
    while (aux && list->compare(aux->key,key)!=0 ) aux=aux->next;

    list->current=aux;
    if (list->head == NULL || list->current == NULL) return NULL;

    if (list->current == list->head) {
        return __popFront(list);
    } else if (list->current == list->tail) {
        return __popBack(list);
    } else {
        if (aux->next != NULL) {
            aux->next->prev = aux->prev;
        }

        if (aux->prev != NULL) {
            aux->prev->next = aux->next;
        }
    }

    void * data = (void *)aux->data;

    list->current = aux->next;

    free(aux);

    list->count -= 1;

    return data;
}

void removeAllSortedMap(SortedMap * list) {
    assert(list != NULL); // list no puede ser NULL.

    while (list->head != NULL) {
        __popFront(list);
    }
}

