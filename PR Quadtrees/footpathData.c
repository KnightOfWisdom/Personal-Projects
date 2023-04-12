/* footpathData.c
*
* Created by Ke Liao 
*
* This module contain functions for the construction of struct 
* storing information about footpath, as well as functions associated
* with the creation, reading of fields,freeing of struct and 
* file printing of struct. 

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "footpathData.h"
#include "point2D.h"
#include "usefulConsts.h"

// Struct definition for storing footpath data
struct footpath{
    int footpath_id;
    char *address, *clue_sa, *asset_type;
    double deltaz, distance, grade1in;
    int mcc_id, mccid_int;
    double rlmax, rlmin;
    char *segside;
    int statusid, streetid, street_group;
    double start_lat,start_lon,end_lat,end_lon;
};


/* Read a line of the csv file containing data for footpath and return the 
pointer containing struct with data of the footpath. Returns Null if 
reading fails. */
footpath_t *footpath_read(FILE *data_file){    
    
    footpath_t *footpath = NULL; 
    int footpath_id;
    double mcc_id, mccid_int;    // take into account the .0 in input
    double statusid, streetid, street_group;  // take into account .0 in input


    // Scan for footpath id and end the scan if at end of file
    if (fscanf(data_file, "%d", &footpath_id) == EOF){
        return footpath;
    }else{
        footpath = malloc(sizeof(*footpath));
        assert(footpath != NULL);
        footpath->footpath_id=footpath_id;
    }

    // Scan for the other fields
    footpath->address = str_field_read(data_file);
    footpath->clue_sa = str_field_read(data_file);
    footpath->asset_type = str_field_read(data_file);
    fscanf(data_file, "%lf,%lf", &(footpath->deltaz), &(footpath->distance));
    fscanf(data_file, ",%lf", &(footpath->grade1in));
    fscanf(data_file, ",%lf,%lf", &mcc_id, &mccid_int);
    fscanf(data_file, ",%lf,%lf", &(footpath->rlmax), &(footpath->rlmin));
    footpath->segside = str_field_read(data_file);
    fscanf(data_file, "%lf,%lf,%lf", &statusid, &streetid, &street_group);
    fscanf(data_file, ",%lf,%lf,%lf,%lf", &(footpath->start_lat), 
        &(footpath->start_lon), &(footpath->end_lat), &(footpath->end_lon));

    // Now assign the other fields to the struct
    footpath->mcc_id = (int)mcc_id;
    footpath->mccid_int = (int)mccid_int;
    footpath->statusid = (int)statusid;
    footpath->streetid = (int)streetid;
    footpath->street_group = (int)street_group;
    return footpath;
}


/* Function for reading the string fields*/
char *str_field_read(FILE *f){
    char *field;
    int curr_char;
    int nchar=0; // number of characters read
    int maxsize=1; 
    field = (char*)malloc(sizeof(char));
    assert(field != NULL);

    // Skip the first comma 
    curr_char = fgetc(f);
    if (curr_char == ','){
        curr_char = fgetc(f);
    }

    // Read the rest of the field.
    if (curr_char == '"'){
        // For cases where field delimited by " read until the second "
        while((curr_char=fgetc(f))!='"'){
            
            // Allocate more space if  required
            if (nchar >= maxsize){
                maxsize *= 2;
                field = (char*)realloc(field, maxsize * sizeof(char));
                assert(field != NULL);
            }

            // Add the character to the string
            field[nchar] = (char)curr_char;
            nchar++;
        }

        curr_char = fgetc(f);  // get rid of , in the file stream

    }else if (curr_char == ','){
        // In case the field is empty, stop reading
        
    }
    else{
        // Read until comma for the other cases

        // Read first char of the field
        field[nchar] = (char)curr_char;
        nchar++;

        // Read rest of the field
        while ((curr_char = fgetc(f)) != ','){
            // Allocate more space if  required
            if (nchar >= maxsize){
                maxsize *= 2;
                field = (char*)realloc(field, maxsize * sizeof(char));
                assert(field != NULL);
            }

            // Add the character to the string
            field[nchar] = (char)curr_char;
            nchar++;
        }
        
    }

    // Add \0 to the end of the string, allocate more spcae if required
    if (nchar==maxsize){
        maxsize++;
        field = (char*)realloc(field, maxsize * sizeof(char));
    }
    field[nchar] = '\0';
    nchar++;
    field = (char*)realloc(field, nchar * sizeof(char));
    assert(field);

    return field;
}


/* Compare the footpath ids of two footpath */
int footpath_id_cmp(footpath_t *footpath1, footpath_t *footpath2){
    if (footpath1->footpath_id == footpath2->footpath_id){
        return EQUALS;
    }else if(footpath1->footpath_id < footpath2->footpath_id){
        return SMALLER_THAN;
    }else{
        return GREATER_THAN;
    }
}

/*Print out the record's data within the struct, to the file 
pointed to by the file pointer f as per format required.*/ 
void data_print(footpath_t *record, FILE *f){
    assert(record != NULL);
    fprintf(f, "--> footpath_id: %d ||", record->footpath_id);
    fprintf(f, " address: %s ||", record->address);
    fprintf(f, " clue_sa: %s ||", record->clue_sa);
    fprintf(f, " asset_type: %s ||", record->asset_type);
    fprintf(f, " deltaz: %f || distance: ", record->deltaz);
    fprintf(f, "%f|| grade1in: %f|| ", record->distance, record->grade1in);
    fprintf(f, "mcc_id: %d || mcc_int: %d", record->mcc_id, record->mccid_int);
    fprintf(f, " || rlmax: %f || rlmin: %f ||", record->rlmax, record->rlmin);
    fprintf(f, " segside: %s || statusid: ", record->segside);
    fprintf(f, "%d || streetid: %d ||", record->statusid, record->streetid);
    fprintf(f, " street_group : %d || start_lat: ", record->street_group);
    fprintf(f, "%f || start_lon: %f ||", record->start_lat, record->start_lon);
    fprintf(f, " end_lat: %f || end lon: ", record->end_lat);
    fprintf(f, "%f ||\n", record->end_lon);
}   

/* Get start point of footpath */
point_t *get_start_point(footpath_t *record){
    return point_creator(record->start_lon, record->start_lat);
}

/* Get end point of footpath */
point_t *get_end_point(footpath_t *record){
    return point_creator(record->end_lon, record->end_lat);
}

/*Free the record and string within*/
void data_free(footpath_t *record){
    free(record->address);
    free(record->clue_sa);
    free(record->asset_type);
    free(record->segside);
    free(record);
}


/* Function for getting the address*/
char *get_address(footpath_t *record){
    return record->address;
}


/* Function for getting the grade1in field */
double get_grade1in(footpath_t *record){
    return record->grade1in;
}


/* Add record to an array of records, ensuring its sorted by the footpath id*/
int sorted_record_add(footpath_t **arr, footpath_t *record, int num_ele){
    
    // Find insert index
    int insert_idx = UNDEFINED;
    for (int i=0; i < num_ele; i++){
        footpath_t *curr_ele = arr[i];
        int cmp = footpath_id_cmp(record, curr_ele);
        if (cmp == SMALLER_THAN){
            insert_idx = i;
            break;
        }else if (cmp == EQUALS){
            return INSERT_FAILURE; // don't insert duplicates
        }
    }

    // For if records is to be inserted at last element
    if (insert_idx == UNDEFINED){
        insert_idx = num_ele;
    }

    // Insert the element to said index and move other elements to the right
    footpath_t *curr_element = record;
    footpath_t *next_element = NULL;
    for (int i = insert_idx; i <= num_ele; i++){
        
        // Get the element that is to be displaced before insertion
        if (i != num_ele){
            next_element = arr[i];
        }

        // Insert the element and the displace element is next to be inserted
        arr[i] = curr_element;
        curr_element = next_element;
    }
    return INSERT_SUCCESS;
}