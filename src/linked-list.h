#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* compares list node values to the new element that we have to insert */
typedef int (*cmp_func)(void *, void *);

typedef struct list_node
{
	void *data;
	struct list_node *next;
} list_node_t;

typedef struct linked_list
{
	list_node_t *head; /*last elem of the list */
	list_node_t *tail; /*first elem of the list */
	int size;
	cmp_func compare;
} linked_list_t;

linked_list_t *linked_list_init(cmp_func);
void linked_list_clear(linked_list_t *);
void linked_list_free(linked_list_t *);
int linked_list_sorted_insert(linked_list_t *, void *);
const void *linked_list_get_element_at(linked_list_t *, int);

#endif
