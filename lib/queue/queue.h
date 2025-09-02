#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../log/log.h"

#ifndef TRUE_FALSE
    #define TRUE 0x1
    #define FALSE 0x0
#endif

#define QUEUE_ENTRY_EXIT_LOG 1
#define QUEUE_MAX_SIZE_BOUND 1

#if QUEUE_MAX_SIZE_BOUND == 1
    #define QUEUE_MAX_SIZE_LIMIT 256 
#endif

#ifndef __is_null
    #define __is_null(ptr)  ((ptr) == NULL)
#endif

#ifndef __is_not_null
    #define __is_not_null(ptr)  ((ptr) != NULL)
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define QDATA_TYPE      void
#define QDATASIZE_TYPE  uint8_t  
#define QSIZE_TYPE      uint32_t  

typedef QDATA_TYPE      qData_t;
typedef QSIZE_TYPE      qSize_t;
typedef QDATASIZE_TYPE  qDataSize_t;

typedef struct qNode_t {
    qData_t* data_ptr;
    qSize_t  data_size;
    struct qNode_t* next;
    struct qNode_t* prev;
} qNode_t;

typedef struct Queue_t{
    qNode_t* head;
    qNode_t* tail;
    qSize_t size;
} Queue_t;

qNode_t* qCreateNode(qNode_t* prev, qNode_t* next,  qData_t* data, qDataSize_t data_size);
qNode_t* qAppendNode(qNode_t* currNode, qData_t* data, qDataSize_t data_size);
void qInit(Queue_t** qPtr);
void qPush(Queue_t* q, qData_t* data, qDataSize_t data_size);
void qEnqueue(Queue_t* q, qData_t* data, qDataSize_t data_size);
void qDequeue(Queue_t* q, qData_t* data, qDataSize_t data_size);
void qPop(Queue_t* q, qData_t* data, qDataSize_t data_size);
qNode_t* qHead(Queue_t* q);
qNode_t* qTail(Queue_t* q);
qSize_t  qSize(Queue_t* q);
uint8_t  qIsEmpty(Queue_t* q);
void qFree(Queue_t* q);
#endif
