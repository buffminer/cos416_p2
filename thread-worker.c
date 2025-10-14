// File:	thread-worker.c
// List all group member's name: Branin Podolski, Matt Rieter
// username of iLab:
// iLab Server:

#include "thread-worker.h"
#define STACK_SIZE SIGSTKSZ
#define DEBUG 1

// Global counter for total context switches and
// average turn around and response time
long tot_cntx_switches = 0;
double avg_turn_time = 0;
double avg_resp_time = 0;
long next_thread_id = 0;
runqueue *rq;

// INITAILIZE ALL YOUR OTHER VARIABLES HERE
// YOUR CODE HERE
void dputs(char *s)
{
	if (DEBUG)
	{
		fprintf(stderr, "%s\n", s);
	}
}

/* create a new thread */
int worker_create(worker_t *thread, pthread_attr_t *attr,
				  void *(*function)(void *), void *arg)
{

	// - create Thread Control Block (TCB)
	// - create and initialize the context of this worker thread
	// - allocate space of stack for this thread to run
	// after everything is set, push this thread into run queue and
	// - make it ready for the execution.

	if (rq == NULL)
	{
		rq = (runqueue *)malloc(sizeof(runqueue));
		init_runqueue(rq);
	}

	// Create TCB for the new worker thread
	tcb *newWorkerTcb = (tcb *)malloc(sizeof(tcb));

	// Initialize context for new thread
	newWorkerTcb->ctx = (ucontext_t *)malloc(sizeof(ucontext_t));
	getcontext(newWorkerTcb->ctx);
	newWorkerTcb->ctx->uc_link = NULL;
	dputs("Context initialized");

	// Allocate and initialize context stack
	newWorkerTcb->ctx->uc_stack.ss_sp = malloc(STACK_SIZE);
	newWorkerTcb->ctx->uc_stack.ss_size = STACK_SIZE;
	newWorkerTcb->ctx->uc_stack.ss_flags = 0;
	makecontext(newWorkerTcb->ctx, (void (*)(void))function, 1, arg);
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
	enqueue(rq, newWorkerTcb);
	return 0;
}

/* give CPU possession to other user-level worker threads voluntarily */
int worker_yield()
{

	// - change worker thread's state from Running to Ready
	// - save context of this thread to its thread control block
	// - switch from thread context to scheduler context

	// YOUR CODE HERE

	return 0;
};

/* terminate a thread */
void worker_exit(void *value_ptr)
{
	// - de-allocate any dynamic memory created when starting this thread

	// YOUR CODE HERE
	worker_t *workerID = (worker_t *)value_ptr;

	thread_node *node = remove_id(rq, *workerID);

	if (node == NULL)
	{
		dputs("Thread does not exist");
		return;
	}

	tcb *workerTcb = node->thread;
	dputs("casting thread value pointer to TCB");
	workerTcb->status = TERMINATED;
	dputs("Thread status set to TERMINATED");
	free(workerTcb->ctx->uc_stack.ss_sp);
	dputs("Freed thread stack");
	free(workerTcb->ctx);
	dputs("Freed thread context");
	free(workerTcb);
	dputs("Freed thread TCB");
	free(node);
};

/* Wait for thread termination */
int worker_join(worker_t thread, void **value_ptr)
{

	// - wait for a specific thread to terminate
	// - de-allocate any dynamic memory created by the joining thread

	// YOUR CODE HERE
	return 0;
};

/* initialize the mutex lock */
int worker_mutex_init(worker_mutex_t *mutex,
					  const pthread_mutexattr_t *mutexattr)
{
	//- initialize data structures for this mutex

	// YOUR CODE HERE
	mutex->locked = 0;
	mutex->owner = NULL;
	mutex->waiting_threads = NULL;
	return 0;
};

/* aquire the mutex lock */
int worker_mutex_lock(worker_mutex_t *mutex)
{

	// - use the built-in test-and-set atomic function to test the mutex
	// - if the mutex is acquired successfully, enter the critical section
	// - if acquiring mutex fails, push current thread into block list and
	// context switch to the scheduler thread

	// YOUR CODE HERE

	while (__sync_lock_test_and_set(&mutex->locked, 1))
		;
	return 0;
};

/* release the mutex lock */
int worker_mutex_unlock(worker_mutex_t *mutex)
{
	// - release mutex and make it available again.
	// - put threads in block list to run queue
	// so that they could compete for mutex later.

	// YOUR CODE HERE
	return 0;
};

/* destroy the mutex */
int worker_mutex_destroy(worker_mutex_t *mutex)
{
	// - de-allocate dynamic memory created in worker_mutex_init

	return 0;
};

/* Pre-emptive Shortest Job First (POLICY_PSJF) scheduling algorithm */
static void sched_psjf()
{
	// - your own implementation of PSJF
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE
}

/* Preemptive MLFQ scheduling algorithm */
static void sched_mlfq()
{
	// - your own implementation of MLFQ
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE

	/* Step-by-step guidances */
	// Step1: Calculate the time current thread actually ran
	// Step2.1: If current thread uses up its allotment, demote it to the low priority queue (Rule 4)
	// Step2.2: Otherwise, push the thread back to its origin queue
	// Step3: If time period S passes, promote all threads to the topmost queue (Rule 5)
	// Step4: Apply RR on the topmost queue with entries and run next thread
}

/* Completely fair scheduling algorithm */
static void sched_cfs()
{
	// - your own implementation of CFS
	// (feel free to modify arguments and return types)

	// YOUR CODE HERE

	/* Step-by-step guidances */

	// Step1: Update current thread's vruntime by adding the time it actually ran
	// Step2: Insert current thread into the runqueue (min heap)
	// Step3: Pop the runqueue to get the thread with a minimum vruntime
	// Step4: Calculate time slice based on target_latency (TARGET_LATENCY), number of threads within the runqueue
	// Step5: If the ideal time slice is smaller than minimum_granularity (MIN_SCHED_GRN), use MIN_SCHED_GRN instead
	// Step5: Setup next time interrupt based on the time slice
	// Step6: Run the selected thread
}

/* scheduler */
static void schedule()
{
	// - every time a timer interrupt occurs, your worker thread library
	// should be contexted switched from a thread context to this
	// schedule() function

	// YOUR CODE HERE

	// - invoke scheduling algorithms according to the policy (PSJF or MLFQ or CFS)
#if defined(PSJF)
	sched_psjf();
#elif defined(MLFQ)
	sched_mlfq();
#elif defined(CFS)
	sched_cfs();
#else
	// # error "Define one of PSJF, MLFQ, or CFS when compiling. e.g. make SCHED=MLFQ"
#endif
}

// DO NOT MODIFY THIS FUNCTION
/* Function to print global statistics. Do not modify this function.*/
void print_app_stats(void)
{

	fprintf(stderr, "Total context switches %ld \n", tot_cntx_switches);
	fprintf(stderr, "Average turnaround time %lf \n", avg_turn_time);
	fprintf(stderr, "Average response time  %lf \n", avg_resp_time);
}

// Feel free to add any other functions you need

// YOUR CODE HERE
