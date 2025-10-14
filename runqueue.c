#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "thread-worker.h"

typedef struct Runqueue {
    struct thread_node* head;
    struct thread_node* tail;
} Runqueue;

typedef struct thread_node {
    tcb* thread;
    struct thread_node* next_thread;
} thread_node;

void init_runqueue(Runqueue *rq) {
    // - initialize your run queue
    rq->head = NULL;
    rq->tail = NULL;
}

void enqueue(Runqueue *rq, tcb* thread) {
    thread_node *new_node = (thread_node *)malloc(sizeof(thread_node));
    new_node->thread = thread;
    new_node->next_thread = NULL;

    if(rq->head == NULL) {
        rq->head = new_node;
    }

    if (rq->tail == NULL) {
        rq->tail = new_node;
    } else {
        rq->tail->next_thread = new_node;
        rq->tail = new_node;
    }
}

thread_node dequeue(Runqueue *rq) {
    if (rq->head == NULL) {
        return NULL;
    }

    thread_node *tn = rq->head;
    rq->head = rq->head->next;

    return tn;
}