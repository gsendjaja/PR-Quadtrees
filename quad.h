#ifndef _QUAD_H_
#define _QUAD_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "data.h"
#include "array.h"

long double lfabs(long double ld);
point2D_t *newPoint(long double *x, long double *y);
rectangle2D_t *newRectangle(long double *x, long double *y, long double *x1, long double *y1);
quadtreeNode_t* createTree(rectangle2D_t *rectangle);
int inRectangle(point2D_t *point, quadtreeNode_t *node);
int determineQuadrant(point2D_t *point, quadtreeNode_t *tree);
void divideTree(quadtreeNode_t *node, int section);
void insertPoint(quadtreeNode_t *node, dataPoint_t *new_data);
void convertQuad(int i);
void print_attributes(FILE *f, footpath_t *path);
void searchPoint(quadtreeNode_t *node, point2D_t *point, FILE *f);
int rectangleOverlap(rectangle2D_t *rect1, rectangle2D_t *rect2);
void buildTree(quadtreeNode_t *tree, footpath_t *paths, list_t *listFoot);
void rangeQuery(quadtreeNode_t *node, rectangle2D_t *rectangle, array_t *unique, char *input_string);
void freeData(dataPoint_t *data);
void freeRectangle(rectangle2D_t *rectangle);
void freeTree(quadtreeNode_t *tree);
int inRectangle2(point2D_t *point, rectangle2D_t *rectangle);
#endif
