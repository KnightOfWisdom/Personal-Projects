#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_
#include "point2D.h"

#define SW_QUADRANT 0
#define NW_QUADRANT 1
#define NE_QUADRANT 2
#define SE_QUADRANT 3

typedef struct rectangle rectangle_t;

rectangle_t *rectangle_create (point_t *bottomleft, point_t *topright);
int in_rectangle(rectangle_t *rectangle, point_t *point);
int rectangle_overlap(rectangle_t *rectangle1, rectangle_t *rectangle2);
void rectangle_free(rectangle_t *rectangle);
int determine_quadrant(rectangle_t *rectangle, point_t *point);
rectangle_t *quadrant_assign(rectangle_t *rect, int quadrant);
#endif