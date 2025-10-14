#define _XOPEN_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <ucontext.h>
#include "../thread-worker.h"
#include "../runqueue.c"

/* A scratch program template on which to call and
 * test thread-worker library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */

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
    // worker_exit(&thread1);
    // worker_exit(&thread2);
    // worker_exit(&thread3);

    Runqueue* q;
    init_runqueue(&q);

	return 0;
}