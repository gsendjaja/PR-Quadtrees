#ifndef _ARRAY_H_
#define _ARRAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "data.h"
#include "structures.h"

int get_footpathID(footpath_t *path);
int id_compare(footpath_t *a, footpath_t *b);
array_t *create_array();
void arrayShrink(array_t *arr);
void arrayEnsureSize(array_t *arr);
void sortedArrayInsert(array_t *arr, footpath_t *B);
void arrayFree(array_t *arr);
void uniqueInsert(array_t *old, footpath_t *new);


#endif
