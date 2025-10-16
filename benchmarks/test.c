// File:	test.c
// List all group member's name: Branin Podolski, Matt Rieter
// username of iLab:
// iLab Server:

#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <ucontext.h>
#include "../thread-worker.h"
// #include "../runqueue.c"

int counter;
ucontext_t main_ctx;

/* A scratch program template on which to call and
 * test thread-worker library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */

void *test_func(void *arg)
{
    printf("Test thread executing.\n");
    counter += 1;
    printf("Counter: %d\n\n", counter);
    worker_exit(NULL);
}

int main(int argc, char **argv)
{
    counter = 0;

    getcontext(&main_ctx);

    // Declare threads
    worker_t thread1;
    worker_t thread2;
    worker_t thread3;

    // Initialize threads
    worker_create(&thread1, NULL, &test_func, NULL);
    worker_create(&thread2, NULL, &test_func, NULL);
    worker_create(&thread3, NULL, &test_func, NULL);

    tcb *thread1_tcb = get_thread_tcb(&thread1);
    tcb *thread2_tcb = get_thread_tcb(&thread2);
    tcb *thread3_tcb = get_thread_tcb(&thread3);

    swapcontext(&main_ctx, &scheduler_context);

    return 0;
}