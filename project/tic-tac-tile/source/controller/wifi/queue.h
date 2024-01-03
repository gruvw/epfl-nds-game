#include "packet.h"

#ifndef INCLUDE_QUEUE

#define INCLUDE_QUEUE

// === Queue Settings ===

#define MAX_QUEUE_SIZE 10

typedef Packet Type;  // must be a typedef struct

// === Queue Definition ===

// Circular Queue structure
typedef struct {
    Type data[MAX_QUEUE_SIZE];
    size_t front, rear;
} Queue;

#endif

// === Queue Interface ===

bool is_empty(Queue * queue);
bool is_full(Queue * queue);
void enqueue(Queue * queue, Type item);
Type peek(Queue * queue);
Type dequeue(Queue * queue);
