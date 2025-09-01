#include "queue.h"
#include <string.h>

qNode_t* createNode(qNode_t* prev, qNode_t* next,  qData_t* data, qDataSize_t data_size){
    qNode_t* node = (qNode_t*)malloc(sizeof(qNode_t));
    if(__is_null(node)) {return NULL;}
    node->data_ptr = (void*) malloc(data_size);
    node->data_ptr = malloc(data_size);
    if(__is_null(node->data_ptr)) {
        free(node);
        return NULL;
    }
    node->data_size = data_size;
    node->next      = next;
    node->prev      = prev;
    if(data) memcpy(node->data_ptr, data, data_size);
    return node;
}

qNode_t* appendNode(qNode_t* currNode, qData_t* data, qDataSize_t data_size){
    /// NULL
    if(__is_null(currNode)) return NULL;
    /// TAIL
    if(__is_null(currNode->next)){
        qNode_t* newNode = createNode(currNode, NULL, data, data_size);
        if(newNode){
            currNode->next = newNode;
            return newNode;
        }else{
            return NULL;
        }
    }
    /// MID
    qNode_t* nextNode = currNode->next;
    qNode_t* newNode  = createNode(currNode, nextNode, data, data_size);
    if(newNode){
        currNode->next = newNode;
        nextNode->prev = newNode;
        return newNode;
    }
    return NULL;
}

void qInit(Queue_t** qPtr){
    if (__is_null(qPtr)){
        fprintf(stderr, "[qInit] <qPtr> is NULL!\n");
        return;
    }
    (*qPtr) = (Queue_t*) malloc(sizeof(Queue_t));
    (*qPtr)->head = NULL;
    (*qPtr)->tail = NULL;
    (*qPtr)->size = 0;
}

void qPush(Queue_t* q, qData_t* data, qDataSize_t data_size){
    qNode_t* newNode;
    if (__is_null(q->head)) {
        newNode = createNode(NULL, NULL, data, data_size);
        if(newNode) q->head = q->tail = newNode;
    } else {
        newNode = appendNode(q->tail, data, data_size);
        if(newNode) q->tail = newNode;
    }
    if(newNode) q->size++;
}

void qEnqueue(Queue_t* q, qData_t* data, qDataSize_t data_size){
    qPush(q, data, data_size);
}

void qDequeue(Queue_t* q, qData_t* data, qDataSize_t data_size){
    if(__is_null(q) || __is_null(q->head)) {
        fprintf(stderr, "[qDequeue] Queue underflow!\n");
        return;
    }
    qNode_t* currHead = q->head;
    if(currHead->data_ptr) 
        memcpy(data, currHead->data_ptr, min(currHead->data_size, data_size));
    
    if(__is_not_null(currHead->next)){
        q->head              = currHead->next;
        currHead->next->prev = NULL;
    } else {
        q->head = q->tail = NULL;
    }
    q->size--;
    free(currHead->data_ptr);
    free(currHead);
    return;
}

void qPop(Queue_t* q, qData_t* data, qDataSize_t data_size) {
    if (__is_null(q) || __is_null(q->tail)) {
        fprintf(stderr, "[qPop] Queue underflow!\n");
        return;
    }
    qNode_t* currTail = q->tail;
    if (currTail->data_ptr && currTail->data_size > 0) {
        memcpy(data, currTail->data_ptr, min(currTail->data_size, data_size));
    }
    if (__is_not_null(currTail->prev)) {
        q->tail = currTail->prev;
        q->tail->next = NULL;
    } else {
        q->head = q->tail = NULL;
    }
    q->size--;
    free(currTail->data_ptr);
    free(currTail);
}

qNode_t* qHead(Queue_t* q){ return __is_null(q) ? NULL : q->head; }
qNode_t* qTail(Queue_t* q){ return __is_null(q) ? NULL : q->tail; }
qSize_t  qSize(Queue_t* q){ return __is_null(q) ? 0 : q->size; }

void qFree(Queue_t* q) {
    if (__is_null(q)) {
        fprintf(stderr, "[qFree] Queue is NULL!\n");
        return;
    }
    qNode_t* cur = q->head;
    while (cur) {
        qNode_t* next = cur->next;
        if (cur->data_ptr) {
            free(cur->data_ptr);
        }
        free(cur);
        cur = next;
    }
    q->head = q->tail = NULL;
    q->size = 0;
}

