#ifndef _DATAPOINT_H_
#define _DATAPOINT_H_

typedef struct data_point data_point_t;

data_point_t *data_point_create(point_t *point_loc);
void record_dt_point_add(data_point_t *dt_point, footpath_t *record);
point_t *get_dt_point_loc(data_point_t *dt_point);
void data_point_record_print(data_point_t *dt_point, FILE *f);
void data_point_free(data_point_t *dt_point);
footpath_t **get_record_list(data_point_t *dt_point);
int get_num_stored(data_point_t *dt_point);
#endif