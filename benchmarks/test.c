#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <ucontext.h>
// #include "../thread-worker.h"

/* A scratch program template on which to call and
 * test thread-worker library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */


typedef uint worker_t;

typedef enum {
	READY, 
	RUNNING, 
	WAITING,
	TERMINATED
} thread_state;

typedef struct TCB {
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

#define STACK_SIZE SIGSTKSZ
#define DEBUG 1

//Global counter for total context switches and 
//average turn around and response time
long tot_cntx_switches=0;
double avg_turn_time=0;
double avg_resp_time=0;
long next_thread_id=0;


// INITAILIZE ALL YOUR OTHER VARIABLES HERE
// YOUR CODE HERE
void dputs(char *s) {
	if (DEBUG) {
		fprintf(stderr, "%s\n", s);
	}
}

/* create a new thread */
int worker_create(worker_t * thread, pthread_attr_t * attr, 
                      void *(*function)(void*), void * arg) {

	// - create Thread Control Block (TCB)
	// - create and initialize the context of this worker thread
	// - allocate space of stack for this thread to run
	// after everything is set, push this thread into run queue and 
	// - make it ready for the execution.

	// Create TCB for the new worker thread
	tcb* newWorkerTcb = (tcb*)malloc(sizeof(tcb));

	// Initialize context for new thread
	newWorkerTcb->ctx = (ucontext_t *)malloc(sizeof(ucontext_t));
	getcontext(newWorkerTcb->ctx);
	newWorkerTcb->ctx->uc_link = NULL;
	dputs("Context initialized");

	// Allocate and initialize context stack
	newWorkerTcb->ctx->uc_stack.ss_sp = malloc(STACK_SIZE);
	newWorkerTcb->ctx->uc_stack.ss_size = STACK_SIZE;
	newWorkerTcb->ctx->uc_stack.ss_flags = 0;
	makecontext(newWorkerTcb->ctx, (void (*)(void)) function, 1, arg);
	dputs("Stack allocated and context modified");

	// Initialize other TCB data
    *thread = next_thread_id++;
	newWorkerTcb->id = *thread;
	newWorkerTcb->status = READY;
	dputs("State and ID set to READY");
    printf("New thread created with ID: %u\n", newWorkerTcb->id);
    printf("Thread status: %d\n", newWorkerTcb->status);

	// TODO: Give a real priority value
	newWorkerTcb->priority = 0; // Highest priority for MLFQ
	
    return 0;
};

void *test_func(void *arg) {
    printf("Test thread executing.\n");
    return NULL;
}

int main(int argc, char **argv) {

	/* Implement HERE */
	worker_t thread1;
    worker_t thread2;
    worker_t thread3;
    worker_create(&thread1, NULL, &test_func, NULL);
    worker_create(&thread2, NULL, &test_func, NULL);
    worker_create(&thread3, NULL, &test_func, NULL);

	return 0;
}