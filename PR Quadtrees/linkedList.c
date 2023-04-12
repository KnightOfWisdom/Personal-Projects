/* linkedList.c
*
* Created by Ke Liao (keliao@student.unimelb.edu.au) 05/09/2022
*
* This module contain functions for the construction of linked list for 
* the footpath records.
*
* Note: this was modifified from dict.c in assignment1
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "footpathData.h"
#include "linkedList.h"
#include "usefulConsts.h"

// Define the node struct of the list
struct list_node{
    footpath_t *footpath_data;    
    list_node_t *next;
};

// Define the linked list
struct linked_list{
    list_node_t *head;
    list_node_t *foot;
};


/* Create empty list and return pointer associated with said list*/
list_t *empty_list_create(){
    list_t *list;
    list = malloc(sizeof(*list));
    assert(list != NULL);
    list->head = list->foot = NULL;
    return list;
}


/* Free the list and the record stored by the list Also free records if 
record free is TRUE*/
void list_free(list_t *list){
    assert(list != NULL);
    free_nodes(list->head);
    free(list);
}


/* Free the node and nodes pointed to by the node. Also free the record if
the record is TRUE*/
void free_nodes(list_node_t *data_node){
    if(data_node==NULL){
        return;  // No need to free if node already NULL
    }else if (data_node->next != NULL){
        free_nodes(data_node->next);
    }
    data_free(data_node->footpath_data);
    
    free(data_node);
}


/* Add a node end of the list */
void add_node(list_t *list, footpath_t *record){
    
    // Creating the new entry
    list_node_t *new_entry;
    new_entry = malloc(sizeof(*new_entry)); 
    assert(list!=NULL && record != NULL && new_entry !=NULL);
    new_entry->footpath_data = record;
    new_entry->next = NULL;

    // Inserting the new entry
    if (list->foot == NULL){
        // For inserting the first entry
        list->head = list->foot = new_entry;
    }else{
        list->foot->next = new_entry;
        list->foot = new_entry;
    }
}


/* Get the head node of the linked list*/
list_node_t *get_head(list_t *list){
    assert(list != NULL);
    return list->head;
}


/* Get the next node of the node*/
list_node_t *next_node(list_node_t *data_node){
    assert(data_node != NULL);
    return data_node->next;
}


/* Get the footpath datastruct stored within an element node of the list*/
footpath_t *get_footpath_data(list_node_t *data_node){
    assert(data_node != NULL);
    return data_node->footpath_data;
}