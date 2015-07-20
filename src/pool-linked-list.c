#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "pool-linked-list.h"
#define RNDTABLE_SIZE 256
#define NUM_ITER 1000

static void *list_alloc(linked_list_t *list)
{
	if (list->allocated == list->max_capacity)
		return NULL;
	return	&list->pool[list->allocated++]; 
}

static void list_free(void *addr)
{
	list_node_t *n = (list_node_t *)addr;	
	n->next = NULL;
}

linked_list_t *linked_list_init(int max_size, int data_size, cmp_func compare)
{
	linked_list_t *list = malloc(sizeof(linked_list_t));
	if (!list) return NULL;

	list->pool = malloc((sizeof(list_node_t) + data_size) * max_size);
	if (!list->pool)
	{
		free(list);
		return NULL;
	}

	list->max_capacity = max_size;
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->node_size = data_size + sizeof(list_node_t);
	list->allocated = 0;
	list->compare = compare;

	return list;
}


void linked_list_clear(linked_list_t *list)
{
	list_node_t *current = list->tail;
	list_node_t *next = NULL;

	while (current != NULL)
	{
		next = current->next;
		list_free(current);
		current = next;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void linked_list_free(linked_list_t *list)
{
	linked_list_clear(list);	
	free(list->pool);
	free(list);
}

int linked_list_sorted_insert(linked_list_t *list, double elem)
{
	list_node_t *node = list_alloc(list);
	if (!node) return 0;
	
	node->distance = elem;
	node->next = NULL;

	if (list->size == 0)
	{
		list->tail = node;
		list->head = node;	
		list->size++;

		return 1;
	}

	/*elem is smaller than tail, set new tail; */
	if (list->compare(&list->tail->distance, &elem))
	{
		node->next = list->tail;
		list->tail = node;
		list->size++;
		return 1;
	}

	/* elem is bigger than head, set new head */
	if (list->compare(&elem, &list->head->distance))
	{
		list->head->next = node;
		list->head = node;
		list->size++;
		return 1;
	}

	list_node_t *current = list->tail->next; 
	list_node_t *last = list->tail;

	while (current != NULL)
	{
		if (list->compare(&current->distance, &elem))
		{
			node->next = current;
			last->next = node; 
			list->size++;
			return 1;
		}

		last = current;
		current = current->next;
	}

	return 0;
}

double linked_list_get_element_at(linked_list_t *list, int index)
{
	assert(index >= 0 && index < list->size);

	list_node_t *current = list->tail;
	int i;
	for (i = 0; i < index; i++)
		current = current->next;

	return current->distance;
}

static int double_cmp( void *a, void *b)
{
	double *x = (double *) a;
	double *y = (double *) b;
	return *x > *y;
}

void list_print(linked_list_t *list)
{
	list_node_t *current = list->tail;
	while (current != NULL)
	{
		printf("%f\n", current->distance);
		current = current->next;
	}
}

int main()
{
	linked_list_t *list = linked_list_init(NUM_ITER, double_cmp);
	if (!list)
		return 1;

	/* borrowed from a certain oooold video game hehehehe */
	const double rndtable[RNDTABLE_SIZE] = { 
	0,   8, 109, 220, 222, 241, 149, 107,  75, 248, 254, 140,  16,  66 ,
    74,  21, 211,  47,  80, 242, 154,  27, 205, 128, 161,  89,  77,  36 ,
    95, 110,  85,  48, 212, 140, 211, 249,  22,  79, 200,  50,  28, 188 ,
    52, 140, 202, 120,  68, 145,  62,  70, 184, 190,  91, 197, 152, 224 ,
    149, 104,  25, 178, 252, 182, 202, 182, 141, 197,   4,  81, 181, 242 ,
    145,  42,  39, 227, 156, 198, 225, 193, 219,  93, 122, 175, 249,   0 ,
    175, 143,  70, 239,  46, 246, 163,  53, 163, 109, 168, 135,   2, 235 ,
    25,  92,  20, 145, 138,  77,  69, 166,  78, 176, 173, 212, 166, 113 ,
    94, 161,  41,  50, 239,  49, 111, 164,  70,  60,   2,  37, 171,  75 ,
    136, 156,  11,  56,  42, 146, 138, 229,  73, 146,  77,  61,  98, 196 ,
    135, 106,  63, 197, 195,  86,  96, 203, 113, 101, 170, 247, 181, 113 ,
    80, 250, 108,   7, 255, 237, 129, 226,  79, 107, 112, 166, 103, 241 ,
    24, 223, 239, 120, 198,  58,  60,  82, 128,   3, 184,  66, 143, 224 ,
    145, 224,  81, 206, 163,  45,  63,  90, 168, 114,  59,  33, 159,  95 ,
    28, 139, 123,  98, 125, 196,  15,  70, 194, 253,  54,  14, 109, 226 ,
    71,  17, 161,  93, 186,  87, 244, 138,  20,  52, 123, 251,  26,  36 ,
    17,  46,  52, 231, 232,  76,  31, 221,  84,  37, 216, 165, 212, 106 ,
    197, 242,  98,  43,  39, 175, 254, 145, 190,  84, 118, 222, 187, 136 ,
    120, 163, 236, 249
	};

	
	int i, idx;
	for (i = 0; i < NUM_ITER; i++)
	{
		idx = i % RNDTABLE_SIZE;
		linked_list_sorted_insert(list, rndtable[idx]);
	}

	list_print(list);
	linked_list_free(list);

	return 0;
}
