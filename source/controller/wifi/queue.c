#include "queue.h"

#define NEXT(e) (((e) + 1) % QUEUE_SIZE)

// == Queue Functions ===

bool is_queue_empty(Queue * queue) {
    return queue->front == queue->rear;
}

bool is_queue_full(Queue * queue) {
    return NEXT(queue->rear) == queue->front;
}

void enqueue(Queue * queue, Type item) {
    if (is_queue_full(queue)) {
        return;  // cannot enqueue, queue overflow!, drop new item to enqueue
    }

    queue->data[queue->rear] = item;
    queue->rear = NEXT(queue->rear);
}

Type peek(Queue * queue) {
    // Supposed nonempty, otherwise continuously returns same element
    return queue->data[queue->front];
}

Type dequeue(Queue * queue) {
    // Supposed nonempty, otherwise continuously returns same element
    Type value = queue->data[queue->front];

    if (!is_queue_empty(queue)) {
        queue->front = NEXT(queue->front);
    }

    return value;
}
