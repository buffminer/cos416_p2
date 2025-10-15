// File:	runqueue.c
// List all group member's name: Branin Podolski, Matt Rieter
// username of iLab:
// iLab Server:

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "runqueue.h"
#include "thread-worker.h"

void init_runqueue(runqueue *rq)
{
    // - initialize your run queue
    rq->head = NULL;
    rq->tail = NULL;
}

void enqueue(runqueue *rq, tcb *thread)
{
    thread_node *new_node = (thread_node *)malloc(sizeof(thread_node));
    new_node->thread = thread;
    new_node->next_thread = NULL;

    if (rq->head == NULL)
    {
        rq->head = new_node;
    }

    if (rq->tail == NULL)
    {
        rq->tail = new_node;
    }
    else
    {
        rq->tail->next_thread = new_node;
        rq->tail = new_node;
    }
}

thread_node *find_tcb(runqueue *rq, worker_t threadID)
{
    if (rq->head == NULL)
    {
        return NULL;
    }

    thread_node *prev = NULL;
    thread_node *curr = rq->head;

    while (curr != NULL)
    {
        if (curr->thread->id == threadID)
        {
            return curr;
        }
        prev = curr;
        curr = curr->next_thread;
    }
    return NULL;
}

thread_node *remove_id(runqueue *rq, worker_t threadID)
{
    if (rq->head == NULL)
    {
        return NULL;
    }

    thread_node *prev = NULL;
    thread_node *curr = rq->head;

    while (curr != NULL)
    {
        if (curr->thread->id == threadID)
        {
            if (prev == NULL)
            {
                // Removing head
                rq->head = curr->next_thread;
                if (rq->tail == curr)
                    rq->tail = rq->head;
            }
            else
            {

                // Removing thread somewhere else in ll
                prev->next_thread = curr->next_thread;
                if (rq->tail == curr)
                    rq->tail = prev;
            }
            curr->next_thread = NULL;
            return curr;
        }
        prev = curr;
        curr = curr->next_thread;
    }
    return NULL;
}

thread_node *dequeue(runqueue *rq)
{
    if (rq->head == NULL)
    {
        return NULL;
    }

    thread_node *tn = rq->head;
    rq->head = rq->head->next_thread;

    return tn;
}

void print_runqueue(runqueue *rq)
{
    thread_node *curr = rq->head;
    printf("Runqueue: ");
    while (curr != NULL)
    {
        printf("%lu ", (unsigned long)curr->thread->id);
        curr = curr->next_thread;
    }
    printf("\n");
}