#include "packet.h"

// === Queue Settings ===

#define QUEUE_SIZE 10
typedef Packet Type;  // queue elements

// === Queue Definition ===

#ifndef INCLUDE_QUEUE

#define INCLUDE_QUEUE

// Circular Fixed Size Queue structure
typedef struct {
    Type data[QUEUE_SIZE];
    size_t front, rear;
} Queue;

#endif

#define EMPTY_QUEUE ((Queue) { { 0 }, 0, 0 })

// === Queue Interface ===

bool is_queue_empty(Queue * queue);
bool is_queue_full(Queue * queue);
void enqueue(Queue * queue, Type item);
Type peek(Queue * queue);
Type dequeue(Queue * queue);
