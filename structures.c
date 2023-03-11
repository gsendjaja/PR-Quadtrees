#include "structures.h"

typedef struct footpath{
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
} footpath_t;

typedef struct point2D{
    long double x;
    long double y;
} point2D_t;

typedef struct rectangle2D{
    point2D_t *bottom_left;
    point2D_t *top_right;
} rectangle2D_t;

typedef struct array{
	footpath_t **A;      // use as array of footpath_t *, ie. footpath_t *A[]
	int size;           // current array size
	int n;              // current number of elements
} array_t;

struct quadtreeNode {
    int colour;
    rectangle2D_t *rectangle;
    dataPoint_t *data;
    quadtreeNode_t **quadrant;
};


typedef struct dataPoint{
    point2D_t *location;
    array_t *arr_footpath;
} dataPoint_t;


struct node {
    footpath_t *path;
    node_t *next;
}; 

typedef struct list{
    node_t *head;   // point to the front of the linked list
    node_t *foot;   // the back
} list_t;



