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

#define LOG_OUT_OF_SIZE         1 
#define QUEUE_ENTRY_EXIT_LOG    0

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

#ifndef __mask8
    // Set i-th bit as 1-bit of 8 bits datatype (E.g: i=3 <---> 00001000B) 
    #define __mask8(i)                      (((uint8_t)1) << (i))
#endif
#ifndef __mask32
    // Set i-th bit as 1-bit of 32 bits datatype (E.g: i=8 <---> 00_00_01_00H) 
    #define __mask32(i)                     (((uint32_t)1)<< (i))
#endif
#ifndef __mask64
    // Set i-th bit as 1-bit of 64 bits datatype (E.g: i=8 <---> 00_00_00_00_00_00_01_00H) 
    #define __mask64(i)                     (((uint64_t)1)<< (i))
#endif
#ifndef __inv_mask8
    // Set i-th bit as 0-bit of 8 bits datatype (E.g: i=3 <---> 11110111B) 
    #define __inv_mask8(i)                  ((uint8_t)(~((uint8_t)1 << (i))))
#endif
#ifndef __inv_mask32
    // Set i-th bit as 0-bit of 32 bits datatype (E.g: i=8 <---> 11_11_10_11H) 
    #define __inv_mask32(i)                 ((uint32_t)(~((uint32_t)1 << (i))))
#endif
#ifndef __inv_mask64
    // Set i-th bit as 0-bit of 64 bits datatype (E.g: i=8 <---> 11_11_11_11_11_11_10_11H) 
    #define __inv_mask64(i)                 ((uint64_t)(~((uint64_t)1 << (i))))
#endif

#define QDATA_TYPE      void
#define QSIZE_TYPE      uint32_t  
#define QCONFIG_TYPE    uint8_t  
#define QDATASIZE_TYPE  uint8_t  

typedef QDATA_TYPE      qData_t;
typedef QSIZE_TYPE      qSize_t;
typedef QCONFIG_TYPE    qConfig_t;
typedef QDATASIZE_TYPE  qDataSize_t;

typedef struct qNode_t {
    qData_t*        data_ptr;
    qSize_t         data_size;
    struct qNode_t* next;
    struct qNode_t* prev;
} qNode_t;

typedef struct Queue_t{
    qNode_t*        head;
    qNode_t*        tail;
    qSize_t         size;
    qSize_t         limited_size;
    qConfig_t       config;
} Queue_t;
enum QUEUE_MODE{
    MODE_QUEUE = 0,
};
enum QUEUE_CONFIG_BIT{
    QMODE_AUTO_LIMIT     = 0,    // Auto remove item if the size is out
    QMODE_QUEUE_STACK    = 1,    // Remove as stack if 1-bit (pop_back) or queue if 0-bit (pop_head/dequeue)
};

qNode_t* qCreateNode(qNode_t* prev, qNode_t* next,  qData_t* data, qDataSize_t data_size);
qNode_t* qAppendNode(qNode_t* currNode, qData_t* data, qDataSize_t data_size);
void qInit(Queue_t** qPtr);
void qInitAsStack(Queue_t** qPtr);
void limitQueue(Queue_t *q);
void qPush(Queue_t* q, qData_t* data, qDataSize_t data_size);
void qEnqueue(Queue_t* q, qData_t* data, qDataSize_t data_size);
void qDequeue(Queue_t* q, qData_t* data, qDataSize_t data_size);
void qPop(Queue_t* q, qData_t* data, qDataSize_t data_size);
qNode_t* qHead(Queue_t* q);
qNode_t* qTail(Queue_t* q);
qSize_t  qSize(Queue_t* q);
uint8_t  qIsEmpty(Queue_t* q);
void     qFree(Queue_t* q);
void     qConfig(Queue_t* q, qConfig_t newConfig);


#endif
