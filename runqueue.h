// File:	runqueue.h
// List all group member's name: Branin Podolski, Matt Rieter
// username of iLab:
// iLab Server:

#ifndef RUNQUEUE_H
#define RUNQUEUE_H

#define _GNU_SOURCE
#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/* Forward declaration of TCB and definition of worker_t */
typedef struct TCB tcb;
typedef uint worker_t;

typedef struct runqueue
{
    struct thread_node *head;
    struct thread_node *tail;
} runqueue;

typedef struct thread_node
{
    tcb *thread;
    struct thread_node *next_thread;
} thread_node;

void init_runqueue(runqueue *rq);

void print_runqueue(runqueue *rq);

void enqueue(runqueue *rq, tcb *thread);
thread_node *find_tcb(runqueue *rq, worker_t threadID);

thread_node *remove_id(runqueue *rq, worker_t threadID);

thread_node *dequeue(runqueue *rq);

#endif RUNQUEUE_H
