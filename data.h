#ifndef _DATA_H_
#define _DATA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "structures.h"
#define INIT_SIZE 2
#define STAGE_THREE 3
#define STAGE_FOUR 4
#define MAX_FIELD_LEN 128
#define INIT_SIZE 2
#define WHITE 0
#define BLACK 1
#define GREY 2
#define NUM_QUADS 4
#define EPSILON 10e-12
                        
list_t *make_empty_list(void);
list_t *insert_at_foot(list_t *list, footpath_t *record);
int processArgs(int argc, char *argv[]);
char *read_string(FILE *f);
void studentSkipHeaderLine(FILE *f);
footpath_t *read_path(FILE *f);
void print_attributes(FILE *f, footpath_t *path);
void free_data(void *data);
void listFree(list_t *list);

#endif
