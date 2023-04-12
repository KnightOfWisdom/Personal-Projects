#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#include <stdio.h>
#include "footpathData.h"

typedef struct list_node list_node_t;
typedef struct linked_list list_t;


list_t *empty_list_create();
void list_free(list_t *list);
void free_nodes(list_node_t *data_node);
void add_node(list_t *list, footpath_t *record);
list_node_t *get_head(list_t *list);
list_node_t *next_node(list_node_t *node);
footpath_t *get_footpath_data(list_node_t *data_node);
#endif