// File:	thread-worker.h

// List all group member's name: Branin Podolski, Matt Rieter
// username of iLab:
// iLab Server:

#ifndef WORKER_T_H
#define WORKER_T_H

#define _GNU_SOURCE

// gcc required inclusion of this line in order to compile on arm64 mac
#define _XOPEN_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_WORKERS macro */
#define USE_WORKERS 1

/* Targeted latency in milliseconds */
#define TARGET_LATENCY 20

/* Minimum scheduling granularity in milliseconds */
#define MIN_SCHED_GRN 1

/* Time slice quantum in milliseconds */
#define QUANTUM 10

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "runqueue.h"

typedef uint worker_t;

typedef enum
{
	READY,
	RUNNING,
	WAITING,
	TERMINATED
} thread_state;

typedef struct TCB
{
	/* add important states in a thread control block */
	// thread Id
	// thread status
	// thread context
	// thread stack
	// thread priority
	// And more ...

	// YOUR CODE HERE
	worker_t id;
	thread_state status;
	ucontext_t *ctx;
	void *stack;
	int priority;
} tcb;

/* mutex struct definition */
typedef struct worker_mutex_t
{
	/* add something here */

	// YOUR CODE HERE
	volatile int locked;
	worker_t *owner;
	runqueue *waiting_threads;
} worker_mutex_t;

typedef struct scheduler
{
	tcb *current_thread;
	ucontext_t scheduler_context;
	struct runqueue thread_queue;
} scheduler;

/* define your data structures here: */
// Feel free to add your own auxiliary data structures (linked list or queue etc...)

// YOUR CODE HERE

/* Function Declarations: */

/* create a new thread */
int worker_create(worker_t *thread, pthread_attr_t *attr, void *(*function)(void *), void *arg);

/* give CPU pocession to other user level worker threads voluntarily */
int worker_yield();

/* terminate a thread */
void worker_exit(void *value_ptr);

/* wait for thread termination */
int worker_join(worker_t thread, void **value_ptr);

/* initial the mutex lock */
int worker_mutex_init(worker_mutex_t *mutex, const pthread_mutexattr_t
												 *mutexattr);

/* aquire the mutex lock */
int worker_mutex_lock(worker_mutex_t *mutex);

/* release the mutex lock */
int worker_mutex_unlock(worker_mutex_t *mutex);

/* destroy the mutex */
int worker_mutex_destroy(worker_mutex_t *mutex);

void set_current_thread(tcb *thread);

tcb *get_thread_tcb(worker_t *id);

/* Function to print global statistics. Do not modify this function.*/
void print_app_stats(void);

#ifdef USE_WORKERS
#define pthread_t worker_t
#define pthread_mutex_t worker_mutex_t
#define pthread_create worker_create
#define pthread_exit worker_exit
#define pthread_join worker_join
#define pthread_mutex_init worker_mutex_init
#define pthread_mutex_lock worker_mutex_lock
#define pthread_mutex_unlock worker_mutex_unlock
#define pthread_mutex_destroy worker_mutex_destroy
#endif

#endif
