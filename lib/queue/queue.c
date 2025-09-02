#include "queue.h"
#include <string.h>

/**
 * @brief Creates a new queue node.
 * * @param prev Pointer to the previous node.
 * @param next Pointer to the next node.
 * @param data Pointer to the data to be copied into the node.
 * @param data_size Size of the data to be copied.
 * @return qNode_t* Pointer to the newly created node, or NULL on failure.
 */
qNode_t* createNode(qNode_t* prev, qNode_t* next,  qData_t* data, qDataSize_t data_size){
    #if QUEUE_ENTRY_EXIT_LOG == 1
        __sim_entry("createNode(%p, %p, %p, %d)", prev, next, data, data_size);
    #endif
    qNode_t* node = (qNode_t*)malloc(sizeof(qNode_t));
    if(__is_null(node)) {
        #if QUEUE_ENTRY_EXIT_LOG == 1
            __sim_exit("createNode() | Error: node is NULL!");
        #endif
        return NULL;
    }
    node->data_ptr = malloc(data_size);
    if(__is_null(node->data_ptr)) {
        free(node);
        #if QUEUE_ENTRY_EXIT_LOG == 1
            __sim_exit("createNode() | Error: data_ptr is NULL!");
        #endif
        return NULL;
    }
    node->data_size = data_size;
    node->next      = next;
    node->prev      = prev;
    if(data) memcpy(node->data_ptr, data, data_size);
    #if QUEUE_ENTRY_EXIT_LOG == 1
        __sim_exit("createNode() | Success!");
    #endif
    return node;
}

/**
 * @brief Appends a new node after a specified node.
 * * @param currNode The node after which the new node will be inserted.
 * @param data Pointer to the data for the new node.
 * @param data_size Size of the data for the new node.
 * @return qNode_t* Pointer to the newly created node, or NULL on failure.
 */
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

/**
 * @brief Initializes a queue structure.
 * * @param qPtr Double pointer to the queue object to be initialized.
 */
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

/**
 * @brief Pushes a new element to the tail (end) of the queue.
 * * @param q Pointer to the queue.
 * @param data Pointer to the data to be added.
 * @param data_size Size of the data.
 */
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

/**
 * @brief Adds an element to the tail (end) of the queue (FIFO).
 * * @param q Pointer to the queue.
 * @param data Pointer to the data to be added.
 * @param data_size Size of the data.
 */
void qEnqueue(Queue_t* q, qData_t* data, qDataSize_t data_size){
    qPush(q, data, data_size);
}

/**
 * @brief Removes an element from the head (front) of the queue (FIFO).
 * * @param q Pointer to the queue.
 * @param data Pointer to a buffer where the dequeued data will be copied.
 * @param data_size Size of the data buffer.
 */
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

/**
 * @brief Pops an element from the tail (end) of the queue (LIFO).
 * * @param q Pointer to the queue.
 * @param data Pointer to a buffer where the popped data will be copied.
 * @param data_size Size of the data buffer.
 */
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

/**
 * @brief Gets the head node of the queue.
 * * @param q Pointer to the queue.
 * @return qNode_t* Pointer to the head node, or NULL if the queue is empty.
 */
qNode_t* qHead(Queue_t* q){ return __is_null(q) ? NULL : q->head; }

/**
 * @brief Gets the tail node of the queue.
 * * @param q Pointer to the queue.
 * @return qNode_t* Pointer to the tail node, or NULL if the queue is empty.
 */
qNode_t* qTail(Queue_t* q){ return __is_null(q) ? NULL : q->tail; }

/**
 * @brief Gets the current size of the queue.
 * * @param q Pointer to the queue.
 * @return qSize_t The number of elements in the queue.
 */
qSize_t  qSize(Queue_t* q){ return __is_null(q) ? 0 : q->size; }

/**
 * @brief Frees all memory allocated for the queue.
 * * @param q Pointer to the queue to be freed.
 */
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
            cur->data_ptr = NULL;
        }
        free(cur);
        cur = next;
    }
    free(q);
}


uint8_t isEmpty(Queue_t* q){
    if(
        __is_null(q) ||
        __is_null(q->head) ||
        __is_null(q->tail) ||
        q->size == 0
    ) return 1;
    return 0;
}