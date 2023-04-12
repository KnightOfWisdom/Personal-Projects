#ifndef _POINT2D_H_
#define _POINT2D_H_

typedef struct point point_t;

point_t *point_creator(long double longitude, long double latitude);
void point_free(point_t *point);
int point_X_cmp(point_t *point1, point_t *point2);
int point_Y_cmp(point_t *point1, point_t *point2);
int point_cmp(point_t *point1, point_t *point2);
long double get_lon(point_t *point);
long double get_lat(point_t *point);

#endif