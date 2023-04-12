/* quadTree.c
*
* Created by Ke Liao 
*
* This module contains functions which construct the quad tree from a set
* of footpath records. In addition, this module also facilitates the searching
* of the quad tree for records in the same area as the query point or
* all records within the query rectangle. To facilitate the search of 
* records within query rectangle, this module also contain struct for 
* storage of matched records(sorted by footpath id)
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rectangle.h"
#include "footpathData.h"
#include "quadTree.h"
#include "usefulConsts.h"
#include "dataPoint.h"


// struct for storing matched records sorted by footpath id
struct matched_records{
    footpath_t **record_list;  
    int num_ele;    // number of elements in array
    int max_size;   // max array size
};


// Tree Node
struct quadtree_node{
    rectangle_t *rectangle;
    data_point_t *dt_point;
    quadtree_node_t *NW;
    quadtree_node_t *NE;
    quadtree_node_t *SW;
    quadtree_node_t *SE;
};


// Quad Tree
struct quadtree{
    quadtree_node_t *root;
};


/* Create a new quad Tree over a defined bot_left & top_right points*/
quadtree_t *tree_create(point_t *bot_left, point_t *top_right){
    quadtree_t *new_tree;
    new_tree = malloc(sizeof(*new_tree));
    assert(new_tree != NULL);
    rectangle_t *rectangle = rectangle_create(bot_left, top_right);
    new_tree->root = tree_node_create(rectangle);
    return new_tree;
}


/* Create a node with no data point over a defined rectangle*/
quadtree_node_t *tree_node_create(rectangle_t *rectangle){
    quadtree_node_t *new_node;
    new_node = malloc(sizeof(*new_node));
    assert(new_node != NULL);
    new_node->dt_point = NULL;
    new_node->rectangle = rectangle;
    new_node->NW = new_node->NE = new_node->SW = new_node->SE = NULL;
    return new_node;
}


/* Add a record to the tree */
void add_record(quadtree_t *qtree, footpath_t *record){
    
    assert(qtree != NULL);
    assert(qtree->root != NULL);

    /* Insert record by its start point */
    point_t *start_point = get_start_point(record);
    insert_record(qtree->root, record, start_point);

    // Do the same for end point
    point_t *end_point = get_end_point(record);
    insert_record(qtree->root, record, end_point);
}

/* Insert record to the appriate branch on the tree, recursively based on the 
 point attached */
void insert_record(quadtree_node_t *node, footpath_t *record, point_t *point){
    assert(node != NULL);
    int is_leaf = is_leaf_node(node);
    rectangle_t *curr_rectangle = node->rectangle;

    // don't insert if not within rectangle
    if (!in_rectangle(curr_rectangle, point)){
        return;
    }

    // Insert data_point to node recursively
    if(node->dt_point == NULL && is_leaf){
        
        // End point of recursion
        data_point_t *dt_point = data_point_create(point);
        record_dt_point_add(dt_point, record);
        node->dt_point = dt_point;
        
    }else{ 
    
        /* For when the leaf node contains data point */ 
        if (is_leaf){ 

            point_t *node_point_loc = get_dt_point_loc(node->dt_point);
            // Add record to the point if record contain the same point
            if (point_cmp(node_point_loc, point) == EQUALS){
                record_dt_point_add(node->dt_point, record);
                point_free(point);  // No longer needed 
                return;
            }

            // Pass the data point to another quadrant
            int quadrant = determine_quadrant(curr_rectangle, node_point_loc);
            data_point_to_quad(node, quadrant);
            node->dt_point = NULL;  // This node is now an internal node
        }

        // Insert to lower branch
        int record_quadrant = determine_quadrant(curr_rectangle, point);
        record_to_quad(node, record, point, record_quadrant);
    }
}


/* Insert record(rec) to quadrant quad(part of recursive insertion process) */
void record_to_quad(quadtree_node_t *node, footpath_t *record,
                    point_t *point, int quad) {
    
    rectangle_t *curr_rect = node->rectangle;

    // Insert to appropriate quadrant(and create node if null)
    if (quad == SW_QUADRANT){
        if (node->SW == NULL){
            rectangle_t *new_quadrant = quadrant_assign(curr_rect, quad);
            node->SW = tree_node_create(new_quadrant);
        }
        insert_record(node->SW, record, point);
    }else if (quad == NW_QUADRANT){
        if (node->NW == NULL){
            rectangle_t *new_quadrant = quadrant_assign(curr_rect, quad);
            node->NW = tree_node_create(new_quadrant);
        }
        insert_record(node->NW, record, point);
    }else if (quad == NE_QUADRANT){
        if (node->NE == NULL){
            rectangle_t *new_quadrant = quadrant_assign(curr_rect, quad);
            node->NE = tree_node_create(new_quadrant);
        }
        insert_record(node->NE, record, point);
    }else if (quad == SE_QUADRANT){
        if (node->SE == NULL){
            rectangle_t *new_quadrant = quadrant_assign(curr_rect, quad);
            node->SE = tree_node_create(new_quadrant);
        }
        insert_record(node->SE, record, point);
    }
}


/* Insert data_point on the leaf node to the quadrant specified. */
void data_point_to_quad(quadtree_node_t *node, int quad){
    
    assert(is_leaf_node(node));
    data_point_t *dt_point = node->dt_point;
    rectangle_t *curr_rect = node->rectangle;
    rectangle_t *new_quadrant = quadrant_assign(curr_rect, quad);
    
    if (quad == SW_QUADRANT){
        node->SW = tree_node_create(new_quadrant);
        node->SW->dt_point = dt_point;
    }else if(quad == NW_QUADRANT){
        node->NW = tree_node_create(new_quadrant); 
        node->NW->dt_point = dt_point;
    }else if (quad == NE_QUADRANT){
        node->NE = tree_node_create(new_quadrant);
        node->NE->dt_point = dt_point;
    }else if (quad == SE_QUADRANT){
        node->SE = tree_node_create(new_quadrant);
        node->SE->dt_point = dt_point;
    }
}


/*Check if the node is a leaf node*/
int is_leaf_node(quadtree_node_t *data_node){
    int isleaf = TRUE;
    if ((data_node->NE != NULL) || (data_node->NW != NULL)){
        isleaf = FALSE;
    }
    if ((data_node->SE != NULL) || (data_node->SW != NULL)){
        isleaf = FALSE;
    }
    return isleaf;
}


/* Search the tree for the point query, printing out associated outputs*/
void tree_query(quadtree_t *tree, point_t *query, FILE *f){
    tree_node_query(tree->root, query, f);
}


/* Look through tree nodes for the query, printing out associated outputs */
void tree_node_query(quadtree_node_t *node, point_t *query, FILE *f){
    
    // Don't want to query null pointers
    if (node == NULL){
        return;
    }
    
    // Query end if the point's not in range defined by the node's rectangle
    rectangle_t *node_rectangle = node->rectangle;
    if (!in_rectangle(node_rectangle, query)){
        return;
    }

    // Point data only located in leaf nodes
    if (is_leaf_node(node)){
        assert(node->dt_point != NULL);   // Something is wrong if NULL
        data_point_record_print(node->dt_point, f);
        return;  // Query done
    }

    // Direct to the correct quadrant if internal node & print the direction
    int query_quadrant = determine_quadrant(node_rectangle, query);
    if (query_quadrant == SE_QUADRANT){
        printf(" SE");
        tree_node_query(node->SE, query, f);
    }else if (query_quadrant == SW_QUADRANT){
        printf(" SW");
        tree_node_query(node->SW, query, f);
    }else if (query_quadrant == NW_QUADRANT){
        printf(" NW");
        tree_node_query(node->NW, query, f);
    }else{
        printf(" NE");
        tree_node_query(node->NE, query, f);
    }
}


/* Find all foorpath records of the tree within rectangular area inputted and
output required outputs to file and stdout */
void tree_ranged_query(quadtree_t *quadtree, rectangle_t *query, FILE *f){
    
    // End query if query not within scope covered by the tree
    int overlap = rectangle_overlap(query, quadtree->root->rectangle);
    if (overlap == FALSE){
        return;
    }

    matched_records_t *matched_records = record_struct_create();
    range_query(quadtree->root, query, matched_records);
    match_record_output(matched_records, f);
    matched_record_struct_free(matched_records);
}


/* Check the nodes of tree for the footpath records in the query area, storing 
matched records in the records and print out directions explored*/
void range_query(quadtree_node_t *node, rectangle_t *query,
                 matched_records_t *records) {
    
    int isleaf = is_leaf_node(node);
    if (isleaf == TRUE){

        // Check if the records are in the area of the query
        point_t *dt_point_loc = get_dt_point_loc(node->dt_point);
        if (in_rectangle(query, dt_point_loc) == FALSE){
            return;
        }

        // Extract records from overlaping leaf nodes if within query's area
        footpath_t **node_records = get_record_list(node->dt_point);
        int num_records = get_num_stored(node->dt_point);
        for (int i = 0; i < num_records; i++){
            matched_record_insert(records, node_records[i]);
        }
    }else{

        // Explore branches that overlap
        if (node->SW != NULL){
            if(rectangle_overlap(query, node->SW->rectangle) == TRUE){
                printf(" SW");
                range_query(node->SW, query, records);
            }
        }
        if (node->NW != NULL){
            if(rectangle_overlap(query, node->NW->rectangle) == TRUE){
                printf(" NW");
                range_query(node->NW, query, records);
            }
        }
        if (node->NE != NULL){
            if(rectangle_overlap(query, node->NE->rectangle) == TRUE){
                printf(" NE");
                range_query(node->NE, query, records);
            }
        }
        if (node->SE != NULL){
            if(rectangle_overlap(query, node->SE->rectangle) == TRUE){
                printf(" SE");
                range_query(node->SE, query, records);
            }
        }
    }

}


/* Output to file records that are matched */
void match_record_output(matched_records_t *records, FILE *output){
    assert(records != NULL);

    for (int i = 0; i < records->num_ele; i++){
        footpath_t *record = (records->record_list)[i];
        data_print(record, output);
    }
}


/* Create the struct which contains array holding matched records*/
matched_records_t *record_struct_create(){
    matched_records_t *records;
    records = malloc(sizeof(*records));
    assert(records != NULL);
    records->max_size = 1;
    records->num_ele = 0;
    records->record_list = malloc(sizeof(footpath_t*) * records->max_size);
    assert(records->record_list != NULL);
    return records;
}


/* Add the found record to the struct containing array of records */
void matched_record_insert(matched_records_t *records, footpath_t *record){
    
    // Allocate space as required 
    int num_ele = records->num_ele;
    if (num_ele == records->max_size){
        records->max_size *= 2;
        records->record_list = realloc(records->record_list, 
            sizeof(footpath_t*) * records->max_size);
        assert(records->record_list != NULL);
    }

    int try_insert = sorted_record_add(records->record_list, record, num_ele);
    if (try_insert == INSERT_SUCCESS){
        records->num_ele += 1;
    }
    
}


/* Free the struct containing array for matched records */
void matched_record_struct_free(matched_records_t *records){
    free(records->record_list);  // Free records later
    free(records);
}


/*Function for freeing the quad tree*/
void free_quad_tree(quadtree_t *curr_quadtree){
    free_tree_nodes(curr_quadtree->root);
    free(curr_quadtree);
}


/* Function for freeing the tree nodes*/
void free_tree_nodes(quadtree_node_t *tree_node){
    assert(tree_node != NULL);
    rectangle_free(tree_node->rectangle);
    if (tree_node->dt_point != NULL){
        data_point_free(tree_node->dt_point);
    }
    if (tree_node->NE != NULL){
        free_tree_nodes(tree_node->NE);
    }
    if (tree_node->NW != NULL){
        free_tree_nodes(tree_node->NW);
    }
    if (tree_node->SE != NULL){
        free_tree_nodes(tree_node->SE);
    }
    if (tree_node->SW != NULL){
        free_tree_nodes(tree_node->SW);
    }
    free(tree_node);
}