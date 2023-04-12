/* main.c
*
* Created by Ke Liao
*
* This is the main program for the execution of stage 3 or 4, depending on
* the flag included. 
*
* Both stage: Construct quad tree from the inputted footpath records over a
* specified range.
*
* Stage 3: take query containing longitude and latitude 
* 
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "linkedList.h"
#include "footpathData.h"
#include "quadTree.h"
#include "point2D.h"
#include "rectangle.h"

#define DEBUG 0
#define STAGE3 3
#define STAGE4 4
#define STAGE_IDX 1
#define INPUT_FILE 2
#define OUTPUT_FILE 3
#define BOT_LEFT_LON 4
#define BOT_LEFT_LAT 5
#define TOP_RIGHT_LON 6
#define TOP_RIGHT_LAT 7

void stage_3_implementation(quadtree_t *quadtree, FILE *output);
void stage_4_implementation(quadtree_t *quadtree, FILE *output);


int main(int argc, char *argv[]){
    FILE *input_file = fopen(argv[INPUT_FILE],"r");
    FILE *output_file = fopen(argv[OUTPUT_FILE],"w");
    assert(input_file != NULL);
    int stage = atoi(argv[STAGE_IDX]);
    
    // Create the empty quad tree
    long double bot_left_lon, bot_left_lat, top_right_lon, top_right_lat;
    sscanf(argv[BOT_LEFT_LON], "%LF", &bot_left_lon); 
    sscanf(argv[BOT_LEFT_LAT], "%LF", &bot_left_lat);
    sscanf(argv[TOP_RIGHT_LON], "%LF", &top_right_lon);
    sscanf(argv[TOP_RIGHT_LAT], "%LF", &top_right_lat);
    point_t *bot_left = point_creator(bot_left_lon, bot_left_lat);
    point_t *top_right = point_creator(top_right_lon, top_right_lat);
    quadtree_t *quadtree = tree_create(bot_left, top_right);

    // Skip the first line as headers don't contain data
    char a = 'r';
    while((a = fgetc(input_file)) != '\n'){}

    // Creation of linked_list for the footpath data
    list_t *list = empty_list_create();
    footpath_t *record;
    while((record = footpath_read(input_file)) != NULL){
        add_node(list, record);        
    }

    // Add footpath records into quad tree
    list_node_t *curr_record_node = get_head(list);
    while (curr_record_node != NULL){
        add_record(quadtree, get_footpath_data(curr_record_node));
        curr_record_node = next_node(curr_record_node);
    }

    if (stage == STAGE3){
        stage_3_implementation(quadtree, output_file);
    }else if (stage == STAGE4){
        stage_4_implementation(quadtree, output_file);
    }

    free_quad_tree(quadtree);
    list_free(list);
    

    // Close the files after finishing 
    fclose(input_file);
    fclose(output_file);    

}


/* Implementation of stage 3*/
void stage_3_implementation(quadtree_t *quadtree, FILE *output){
    
    char *query = NULL;  // query inputs
    size_t query_len = 0;

    /* Read input point query & perform search & output results */
    while (getline(&query, &query_len, stdin) != EOF){
        
        // Get rid of newline char in query
        sscanf(query, "%[^\n]", query);
        fprintf(output, "%s\n", query);

        // Process query & perform search 
        double query_lon, query_lat;
        sscanf(query, "%lf %lf", &query_lon, &query_lat);
        printf("%s -->", query);
        point_t *query_point = point_creator(query_lon, query_lat);
        tree_query(quadtree, query_point, output);
        printf("\n");

        point_free(query_point);
    }

    free(query);
    query = NULL;
}


/* Implementation of stage 4*/
void stage_4_implementation(quadtree_t *quadtree, FILE *output){
    char *query = NULL;  // query inputs
    size_t query_len = 0;

    /* Read input point query & perform search & output results */
    while (getline(&query, &query_len, stdin) != EOF){
        
        // Get rid of newline char in query
        sscanf(query, "%[^\n]", query);
        fprintf(output, "%s\n", query);
        
        // Process query and perform search
        double left, right, top, bot;
        sscanf(query, "%lf %lf %lf %lf", &left, &bot, &right, &top);
        point_t *bot_left = point_creator(left, bot);
        point_t *top_right = point_creator(right, top);
        rectangle_t *query_rectangle = rectangle_create(bot_left, top_right);
        printf("%s -->", query);
        tree_ranged_query(quadtree, query_rectangle, output);
        printf("\n");

        rectangle_free(query_rectangle); 
    }

    free(query);
    query = NULL;
}