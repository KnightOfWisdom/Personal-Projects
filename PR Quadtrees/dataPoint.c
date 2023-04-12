/* dataPoint.c
*
* Created by Ke Liao
* 
* This module contains function that construct struct which stores
* a list of footpath records and the points the footpath records is located
* at. In addition, this module contain function facilitating extraction of
* information from the data structure as well as insertion of records 
* into the data structure
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "point2D.h"
#include "footpathData.h"
#include "dataPoint.h"
#include "usefulConsts.h"

// Stores a point with its associated records
struct data_point{
    point_t *point_loc;
    footpath_t **record_list;  // array sorted by footpath id
    int num_ele;    // number of elements in array
    int max_size;   // max array size
};


/* Create new data point with no associated record*/
data_point_t *data_point_create(point_t *point_loc){
    data_point_t *new_dt_point = malloc(sizeof(data_point_t));
    assert(new_dt_point != NULL);
    int max_size = 1;
    new_dt_point->max_size = max_size;
    new_dt_point->num_ele = 0; 
    new_dt_point->point_loc = point_loc;
    new_dt_point->record_list = malloc(sizeof(footpath_t*) * max_size);
    return new_dt_point;
}


/* Add a footpath record to data point making sure the array of record stays 
sorted */
void record_dt_point_add(data_point_t *dt_point, footpath_t *record){

    // Malloc more space as required
    int num_ele = dt_point->num_ele;
    if (num_ele == dt_point->max_size){
        dt_point->max_size *= 2;
        dt_point->record_list = realloc(dt_point->record_list, 
            sizeof(footpath_t*) * dt_point->max_size);
        assert(dt_point->record_list != NULL);
    }

    int try_insert = sorted_record_add(dt_point->record_list, record, num_ele);
    if (try_insert == INSERT_SUCCESS){
        dt_point->num_ele += 1;
    }
    
}


/* Get point location stored by the data point */
point_t *get_dt_point_loc(data_point_t *dt_point){
    return dt_point->point_loc;
} 


/* Print footpath records associated with the point to File pointed to by f*/
void data_point_record_print(data_point_t *dt_point, FILE *f){
    for (int i=0; i < dt_point->num_ele; i++){
        data_print((dt_point->record_list)[i],f);
    }
}


/* Free data point*/
void data_point_free(data_point_t *dt_point){
    assert(dt_point != NULL);
    point_free(dt_point->point_loc);
    free(dt_point->record_list);  // Free the records later
    free(dt_point);
}


/* Get list of records stored in the data point*/
footpath_t **get_record_list(data_point_t *dt_point){
    return dt_point->record_list;
}


/* Get the number of records stored in the data point*/
int get_num_stored(data_point_t *dt_point){
    return dt_point->num_ele;
}