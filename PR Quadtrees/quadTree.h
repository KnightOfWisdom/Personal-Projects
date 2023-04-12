#ifndef _QUADTREECREATOR_H_
#define _QUADTREECREATOR_H_
#include "rectangle.h" 

typedef struct quadtree_node quadtree_node_t;
typedef struct quadtree quadtree_t;
typedef struct matched_records matched_records_t;

quadtree_t *tree_create(point_t *bot_left, point_t *top_right);
quadtree_node_t *tree_node_create(rectangle_t *rectangle);
void add_record(quadtree_t *qtree, footpath_t *record);
void insert_record(quadtree_node_t *node, footpath_t *record, point_t *point);
void record_to_quad(quadtree_node_t *node, footpath_t *record,
                    point_t *point, int quad);
void data_point_to_quad(quadtree_node_t *node, int quad);
int is_leaf_node(quadtree_node_t *data_node);
void tree_query(quadtree_t *tree, point_t *query, FILE *f);
void tree_node_query(quadtree_node_t *node, point_t *query, FILE *f);
void tree_ranged_query(quadtree_t *quadtree, rectangle_t *query, FILE *f);
void range_query(quadtree_node_t *node, rectangle_t *query,
                 matched_records_t *records);
void match_record_output(matched_records_t *records, FILE *output);
matched_records_t *record_struct_create();
void matched_record_insert(matched_records_t *records, footpath_t *record);
void matched_record_struct_free(matched_records_t *records);
void free_quad_tree(quadtree_t *curr_quadtree);
void free_tree_nodes(quadtree_node_t *tree_node);

#endif