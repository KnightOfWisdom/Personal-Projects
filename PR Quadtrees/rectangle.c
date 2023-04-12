/* rectangle.c
*
* Created by Ke Liao (keliao@student.unimelb.edu.au) 05/09/2022
*
* This module contains function for construct of structs which contain 
* information that together represents a rectangle. In addition, this module 
* also contains other functions that determines which quadrant a point is in
* as well as checking intersection between rectangle & rectangle/points.
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "point2D.h"
#include "rectangle.h"
#include "usefulConsts.h"


// defines rectangle's & its two main points
struct rectangle{
    point_t *bottomleft;
    point_t *topright;
};


/* Create rectangle */
rectangle_t *rectangle_create (point_t *bottomleft, point_t *topright){
    rectangle_t *new_rectangle = malloc(sizeof(rectangle_t));
    assert(new_rectangle != NULL);
    new_rectangle->bottomleft = bottomleft;
    new_rectangle->topright = topright;
    return new_rectangle;
}


/* Check if a point is inside a rectangle */
int in_rectangle(rectangle_t *rectangle, point_t *point){
    int within_bound = TRUE;

    // Check if point is within left, right, top, bot bound respectively
    if (point_X_cmp(point, rectangle->bottomleft) != GREATER_THAN){
        within_bound = FALSE;
    } else if (point_X_cmp(point, rectangle->topright) == GREATER_THAN){
        within_bound = FALSE;
    } else if (point_Y_cmp(point, rectangle->topright) != SMALLER_THAN){
        within_bound = FALSE;
    }else if (point_Y_cmp(point, rectangle->bottomleft) == SMALLER_THAN){
        within_bound = FALSE;
    }

    return within_bound;
}


/* Check for overlapping of rectangles*/
int rectangle_overlap(rectangle_t *rectangle1, rectangle_t *rectangle2){

    /* Overlap won't occur if one rectangle is above/below another and/or
    rectangle is on the left/right of other.*/
    int overlap = TRUE;

    // Get the rectangles' longitude & latitude values for its sides
    long double rect1_right = get_lon(rectangle1->topright);
    long double rect1_left = get_lon(rectangle1->bottomleft);
    long double rect1_top = get_lat(rectangle1->topright);
    long double rect1_bot = get_lat(rectangle1->bottomleft);
    long double rect2_right = get_lon(rectangle2->topright);
    long double rect2_left = get_lon(rectangle2->bottomleft);
    long double rect2_top = get_lat(rectangle2->topright);
    long double rect2_bot = get_lat(rectangle2->bottomleft);

    if ((rect1_top < rect2_bot) || (rect1_bot >  rect2_top)){
        overlap = FALSE;
    }else if((rect1_right < rect2_left) || (rect1_left > rect2_right)){
        overlap = FALSE;
    }

    return overlap;
}


/* Free the rectangle */
void rectangle_free(rectangle_t *rectangle){
    point_free(rectangle->topright);
    point_free(rectangle->bottomleft);
    free(rectangle);
}


/* Determine which Quadrant the point is in */
int determine_quadrant(rectangle_t *rectangle, point_t *point){
    long double left = get_lon(rectangle->bottomleft);
    long double top = get_lat(rectangle->topright);
    long double right = get_lon(rectangle->topright);
    long double bot = get_lat(rectangle->bottomleft);
    long double longitude_ave = (left + right)/2;
    long double latitude_ave = (top + bot)/2;
    long double point_lon = get_lon(point);
    long double point_lat = get_lat(point);

    if ((point_lon <= longitude_ave) && (point_lat < latitude_ave)){
        return SW_QUADRANT;
    }else if ((point_lon <= longitude_ave) && (point_lat >= latitude_ave)){
        return NW_QUADRANT;
    }else if ((point_lon > longitude_ave) && (point_lat >= latitude_ave)){
        return NE_QUADRANT;
    }else{
        return SE_QUADRANT;
    }

}


/* Return a new rectangle which is the quadrant the point is in */
rectangle_t *quadrant_assign(rectangle_t *rect, int quadrant){

    // Get requisite data point
    long double left = get_lon(rect->bottomleft);
    long double top = get_lat(rect->topright);
    long double right = get_lon(rect->topright);
    long double bot = get_lat(rect->bottomleft);
    long double longitude_ave = (left + right)/2;
    long double latitude_ave = (top + bot)/2;

    // Return the rectangle of the appropriate quadrant
    if (quadrant == SW_QUADRANT){
        point_t *SW_bot_left = point_creator(left, bot);
        point_t *SW_topright = point_creator(longitude_ave, latitude_ave);
        return rectangle_create(SW_bot_left, SW_topright);
    }
    if (quadrant == NW_QUADRANT){
        point_t *NW_bot_left = point_creator(left, latitude_ave);
        point_t *NW_topright = point_creator(longitude_ave, top);
        return rectangle_create(NW_bot_left, NW_topright);
    }
    if (quadrant == NE_QUADRANT){
        point_t *NE_bot_left = point_creator(longitude_ave, latitude_ave);
        point_t *NE_topright = point_creator(right, top);
        return rectangle_create(NE_bot_left, NE_topright);
    }
    if (quadrant == SE_QUADRANT){
        point_t *SE_bot_left = point_creator(longitude_ave, bot);
        point_t *SE_topright = point_creator(right, latitude_ave);
        return rectangle_create(SE_bot_left, SE_topright);
    }

    // For invalid input quadrant number
    return NULL;
}
