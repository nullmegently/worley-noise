#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "linked-list.h"

linked_list_t *linked_list_init(cmp_func compare)
{
	linked_list_t *list = malloc(sizeof(linked_list_t));
	if (!list) return NULL;

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->compare = compare;

	return list;
}


void linked_list_clear(linked_list_t *list)
{
	list_node_t *current = list->tail;
	list_node_t *next = NULL;

	while (current != NULL)
	{
		free(current->data);
		next = current->next;
		free(current);
		current = next;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

void linked_list_free(linked_list_t *list)
{
	linked_list_clear(list);	
	free(list);
}

int linked_list_sorted_insert(linked_list_t *list, void *elem)
{
	list_node_t *node = malloc(sizeof(list_node_t));
	if (!node) return 0;

	node->data = elem;
	node->next = NULL;

	if (list->size == 0)
	{
		list->tail = node;
		list->head = node;	
		list->size++;

		return 1;
	}

	/*elem is smaller than tail, set new tail; */
	if (list->compare(list->tail->data, elem))
	{
		node->next = list->tail;
		list->tail = node;
		list->size++;
		return 1;
	}

	/* elem is bigger than head, set new head */
	if (list->compare(elem, list->head->data))
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
		if (list->compare(current->data, elem))
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

const void *linked_list_get_element_at(linked_list_t *list, int index)
{
	assert(index >= 0 && index < list->size);

	list_node_t *current = list->tail;
	int i;
	for (i = 0; i < index; i++)
		current = current->next;

	return current->data;
}
