#ifndef _FOOTPATHDATA_H_
#define _FOOTPATHDATA_H_
#include <stdio.h>
#include "point2D.h"

// Foot path data struct def
typedef struct footpath footpath_t;

footpath_t *footpath_read(FILE *data_file);
char *str_field_read(FILE *f);
void data_print(footpath_t *record, FILE *f);
void data_free(footpath_t *record);
int footpath_id_cmp(footpath_t *footpath1, footpath_t *footpath2);
char *get_address(footpath_t *record);
double get_grade1in(footpath_t *record);
point_t *get_start_point(footpath_t *record);
point_t *get_end_point(footpath_t *record);
int sorted_record_add(footpath_t **arr, footpath_t *record, int num_ele);
#endif