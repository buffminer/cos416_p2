#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// These are only for the random array/testing purposes
// Will not be relavent for the project
#define MINIMUM 1
#define MAXIMUM 20
#define ARR_LENGTH 12
#define DEBUG 1

// These will be needed for the project
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

// This is going to be the structure that represents the processes
typedef struct
{
    int key;
} Node;

Node *random_array(int size, int min, int max);
void swap(Node *parent, Node *child);
void heapify(Node *arr, int i, int size);
void min_heap(Node *arr, int size);

int main()
{
    Node *arr = random_array(ARR_LENGTH, MINIMUM, MAXIMUM);
    if(DEBUG)
    {
        printf("Before sort: ");
        for(int i = 0; i < ARR_LENGTH; i++)
        {
            printf("%i ", arr[i].key);
        }
        printf("\n");
    }
    
    min_heap(arr, ARR_LENGTH);
    if(DEBUG)
    {
        printf("After sort:  ");
        for(int i = 0; i < ARR_LENGTH; i++)
        {
            printf("%i ", arr[i].key);
        }
        printf("\n");
    }

    free(arr);
    return 0;
}

Node *random_array(int size, int min, int max)
{
    if(max - min + 1 < size)
    {
        fprintf(stderr, "Error: range too small for %d unique numbers.\n", size);
        exit(EXIT_FAILURE);
    }
    srand((unsigned)time(NULL));
    int range = max - min + 1;
    int *seen = calloc(range, sizeof(int));
    if(!seen)
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    Node *arr = (Node *)malloc(size * sizeof(Node));
    if(!arr)
    {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    
    int num;
    for(int i = 0; i < size; i++)
    {
        Node node;
        arr[i] = node;
        do{
            num = rand() % (max - min + 1) + min;
        }while(seen[num - min]);
        seen[num - min] = 1;
        arr[i].key = num;
    }
    return arr;
}

void swap(Node *parent, Node *child)
{
    Node temp = *parent;
    *parent = *child;
    *child = temp;
}

void heapify(Node *arr, int i, int size)
{
    int minimum = i;
    int left = LEFT(i);
    int right = RIGHT(i);

    if(left < size && arr[minimum].key > arr[left].key)
    {
        minimum = left;
    }
    if(right < size && arr[minimum].key > arr[right].key)
    {
        minimum = right;
    }
    if(minimum != i)
    {
        swap(&arr[i], &arr[minimum]);

        heapify(arr, minimum, size);
    }
}

void min_heap(Node *arr, int size)
{
    for(int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(arr, i, size);
    }
}