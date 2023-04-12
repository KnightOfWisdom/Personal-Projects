/* point2D.c
*
* Created by Ke Liao
*
* This module contains functions for construction of struct storing 
* latitude and longitude of a point. This module also contain functions that
* returns information about the point as well the comparison of two points.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "point2D.h"
#include "usefulConsts.h"

struct point{
    long double longitude; // Also known as x-val
    long double latitude; // Also known as y_val
};


/* Create a pointer to a point with supplied longitude & latitude value*/
point_t *point_creator(long double longitude,long double latitude){
    point_t *new_point = malloc(sizeof(point_t));
    assert(new_point != NULL);
    new_point->longitude = longitude;
    new_point->latitude = latitude;
    return new_point;
}


/* Compare two point's logitude (x) */
int point_X_cmp(point_t *point1, point_t *point2){
    if ((point1->longitude) < (point2->longitude)){
        return SMALLER_THAN;
    }else if ((point1->longitude) == (point2->longitude)){
        return EQUALS;
    }else{
        return GREATER_THAN;
    } 
}


/* Compare two point's latitude(y) */
int point_Y_cmp(point_t *point1, point_t *point2){
    if ((point1->latitude) < (point2->latitude)){
        return SMALLER_THAN;
    }else if ((point1->longitude) == (point2->latitude)){
        return EQUALS;
    }else{
        return GREATER_THAN;
    } 
}


/* Check if two points are the same */
int point_cmp(point_t *point1, point_t *point2){
    if((point1->latitude) == (point2->latitude)){
        if((point1->longitude) == (point2->longitude)){
            return EQUALS;
        }
    }
    return NOT_EQUALS;
}


/* Free the point*/
void point_free(point_t *point){
    assert(point != NULL);
    free(point);
}


/* get logititude val of point */
long double get_lon(point_t *point){
    return point->longitude;
}


/* get latitude val of point */
long double get_lat(point_t *point){
    return point->latitude;
}