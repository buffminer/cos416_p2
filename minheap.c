#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// These are only for the random array/testing purposes
// Will not be relavent for the project
#define MINIMUM 1
#define MAXIMUM 20
#define TEST_LENGTH 15
#define DEBUG 1

// These will be needed for the project
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

// This is going to be the structure that represents the threads
typedef struct
{
    int key;
} Node;

// This structure represents the min heap
typedef struct
{
    Node *arr;
    size_t size;
    size_t capacity;
} minHeap;

int heap_push(minHeap *heap, Node node);
int heap_pop(minHeap *heap);
int random_array(minHeap *heap, size_t quantity, int min, int max);
void swap(Node *parent, Node *child);
void bubble_up(minHeap *heap, size_t i);
void bubble_down(minHeap *heap, size_t i);
void build_min_heap(minHeap *heap);

int main()
{
    minHeap heap = {0};
    
    random_array(&heap, TEST_LENGTH, MINIMUM, MAXIMUM);

    if(!heap_pop(&heap)) fprintf(stderr, "Error: no node was popped.");

    free(heap.arr);
    return 0;
}

int heap_push(minHeap *heap, Node node)
{
    if(heap->size == heap->capacity)
    {
        size_t newcap = heap->capacity ? heap->capacity * 2 : 8;
        if (newcap > SIZE_MAX / sizeof(Node)) return 0;
        Node *temp = realloc(heap->arr, newcap * sizeof(Node));
        if(!temp) return 0;
        heap->arr = temp;
        heap->capacity = newcap;
    }
    heap->arr[heap->size++] = node;
    bubble_up(heap, heap->size - 1);
    if(DEBUG)
    {
        printf("%i was added to the heap.\n", node.key);
        printf("Current heap: ");
        for(size_t i = 0; i < heap->size; i++)
        {
            printf("%i ", heap->arr[i].key);
        }
        printf("\nSize: %zu\nCapacity: %zu\n\n", heap->size, heap->capacity);
    }
    return 1;
}

int heap_pop(minHeap *heap)
{
    if(heap->size != 0)
    {
        Node pop = heap->arr[0];
        size_t newlen = heap->size - 1;
        swap(&heap->arr[0], &heap->arr[heap->size - 1]);
        Node *temp = realloc(heap->arr, newlen * sizeof(*temp));
        if(!temp) return 0;
        heap->arr = temp;
        heap->size--;
        bubble_down(heap, 0);
        if(DEBUG)
        {
            printf("%i was removed from the heap.\n", pop.key);
            printf("Current heap: ");
            for(size_t i = 0; i < heap->size; i++)
            {
                printf("%i ", heap->arr[i].key);
            }
            printf("\nSize: %zu\nCapacity: %zu\n\n", heap->size, heap->capacity);
        }
        return 1;
    }
    return 0;
}

int random_array(minHeap *heap, size_t quantity, int min, int max)
{
    int range = max - min + 1;
    if (range < 0 || (size_t)range < quantity) {
        fprintf(stderr, "Error: range too small for %zu unique numbers.\n", quantity);
        return 0;
    }

    int *seen = calloc((size_t)range, sizeof *seen);
    if (!seen){ 
        perror("calloc"); 
        return 0; 
    }

    int ok = 1;
    for (size_t i = 0; i < quantity; i++) {
        int num;
        do { 
            num = rand() % range + min; 
        } while (seen[num - min]);
        seen[num - min] = 1;

        Node node = { .key = num };
        if(!heap_push(heap, node)){ 
            ok = 0; 
            break; 
        }
    }

    free(seen);
    return ok;
}

void swap(Node *parent, Node *child)
{
    Node temp = *parent;
    *parent = *child;
    *child = temp;
}

void bubble_up(minHeap *heap, size_t i)
{
    size_t parent = PARENT(i);
    if(i != 0 && heap->arr[i].key < heap->arr[parent].key)
    {
        swap(&heap->arr[parent], &heap->arr[i]);
        bubble_up(heap, parent);
    }
    
}

void bubble_down(minHeap *heap, size_t i)
{
    size_t minimum = i;
    size_t left = LEFT(i);
    size_t right = RIGHT(i);

    if(left < heap->size && heap->arr[minimum].key > heap->arr[left].key)
    {
        minimum = left;
    }
    if(right < heap->size && heap->arr[minimum].key > heap->arr[right].key)
    {
        minimum = right;
    }
    if(minimum != i)
    {
        swap(&heap->arr[i], &heap->arr[minimum]);

        bubble_down(heap, minimum);
    }
}

void build_min_heap(minHeap *heap)
{
    for(size_t i = heap->size / 2; i-- > 0; )
    {
        bubble_down(heap, i);
    }
}
