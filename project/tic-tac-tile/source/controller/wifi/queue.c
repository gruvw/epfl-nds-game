#include "queue.h"

#define NEXT(q) ((queue->rear + 1) % QUEUE_SIZE)

// == Queue Functions ===

bool is_queue_empty(Queue * queue) {
    return queue->front == queue->rear;
}

bool is_queue_full(Queue * queue) {
    return NEXT(queue) == queue->front;
}

void enqueue(Queue * queue, Type item) {
    if (is_queue_full(queue)) {
        return;  // cannot enqueue, queue overflow!, drop new item to enqueue
    }

    queue->data[queue->rear] = item;
    queue->rear = NEXT(queue);
}

Type peek(Queue * queue) {
    // Supposed non-empty, otherwise continuously returns same element
    return queue->data[queue->front];
}

Type dequeue(Queue * queue) {
    // Supposed non-empty, otherwise continuously returns same element
    Type value = queue->data[queue->front];

    if (queue->front != queue->rear) {  // more elements in the queue
        queue->front = (queue->front + 1) % QUEUE_SIZE;
    }

    return value;
}
