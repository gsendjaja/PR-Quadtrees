#include "quad.h"
#include "data.h"
#include "structures.c"

//code taken from ED post, thank you Nathaniel Putro
//function that returns a long double by the subtraction of a long double from a long double
long double lfabs(long double ld) {
    return (ld > 0) ? ld : -ld;
 }

//allocates memory and returns a quadtreeenode when an rectangle type is given as input
quadtreeNode_t* createTree(rectangle2D_t *rectangle) {
    quadtreeNode_t *node;
    node = malloc(sizeof(*node));
    assert(node);
    node->data = NULL;
    node->rectangle = rectangle;
    node->quadrant = (quadtreeNode_t **)malloc(sizeof(quadtreeNode_t*) * 4);
    assert(node->quadrant);
    for (int i = 0; i < NUM_QUADS;i++) {
        node->quadrant[i] = NULL;
    }
    node->colour = WHITE;
    return node;
}

//allocates memory for a new point and sets the x and y according to input
point2D_t *newPoint(long double *x, long double *y) {
    point2D_t *temp = malloc(sizeof(*temp));
    assert(temp);
    temp->x = *x;
    temp->y = *y;
    return temp;
}

//allocates emory to make a new rectangle and takes long doubles (coordinates) as input
rectangle2D_t *newRectangle(long double *x, long double *y, long double *x1, long double *y1) {
    rectangle2D_t *temp = malloc(sizeof(*temp));
    assert(temp);
    point2D_t *bottomleft = newPoint(x, y);
    assert(bottomleft);
    point2D_t *upperright = newPoint(x1, y1);
    assert(upperright);
    temp->bottom_left = bottomleft;
    temp->top_right =  upperright;
    return temp;
}

//checking if there is a point in that node
int inRectangle(point2D_t *point, quadtreeNode_t *node) {
    if (point->x > node->rectangle->bottom_left->x && point->y >= node->rectangle->bottom_left->y 
        && point->x <= node->rectangle->top_right->x && point->y < node->rectangle->top_right->y) {
        return 1;
    }
    return 0;
}

//returns 1 if input point lies inside the input rectangle
int inRectangle2(point2D_t *point, rectangle2D_t *rectangle) {
    if (point->x > rectangle->bottom_left->x && point->y >= rectangle->bottom_left->y 
        && point->x <= rectangle->top_right->x && point->y < rectangle->top_right->y) {
        return 1;
    }
    return 0;
}

//returns the quadrant number depending on where the input point lies in the input tree
int determineQuadrant(point2D_t *point, quadtreeNode_t *tree) {
    long double mid_x = (tree->rectangle->bottom_left->x + tree->rectangle->top_right->x) / 2;
    long double mid_y = (tree->rectangle->bottom_left->y + tree->rectangle->top_right->y) / 2;
    
    if (inRectangle(point, tree)) {
        if (point->x <= mid_x && point->y < mid_y) {
            return 0;
        } else if (point->x <= mid_x && point->y >= mid_y) {
            return 1;
        } else if (point->x > mid_x && point->y >= mid_y) {
            return 2;
         } else if (point->x > mid_x && point->y < mid_y) {
            return 3;    
        }
    }
    return 0;
}
void buildTree(quadtreeNode_t *tree, footpath_t *paths, list_t *footList) {
    point2D_t *start_point = malloc(sizeof(*start_point));
    assert(start_point);
    point2D_t *end_point = malloc(sizeof(*end_point));
     assert(end_point);
    dataPoint_t *start_data = malloc(sizeof(*start_data));
     assert(start_data);
    dataPoint_t *end_data = malloc(sizeof(*end_data));
     assert(end_data);

    start_point->x = paths->start_lon;
    start_point->y = paths->start_lat;
    end_point->x = paths->end_lon;
    end_point->y = paths->end_lat;
    
    //inserting footpath and point coordinates into datapoint
    start_data->arr_footpath = create_array();
    start_data->location = start_point;
    sortedArrayInsert(start_data->arr_footpath, paths);
    insertPoint(tree, start_data);

    end_data->arr_footpath = create_array();
    end_data->location = end_point;
    sortedArrayInsert(end_data->arr_footpath, paths);
    insertPoint(tree, end_data);
    insert_at_foot(footList, paths);

}

//takes the quadrant as input and creates a new quad tree node with that rectangle as input
void divideTree(quadtreeNode_t *node, int section) {
    long double mid_x = (node->rectangle->bottom_left->x + node->rectangle->top_right->x) / 2;
    long double mid_y = (node->rectangle->bottom_left->y + node->rectangle->top_right->y) / 2;
    if (section == 0) {
        //first quadrant. bottom left is original point while top right is the centre of the bigger rectangle
        rectangle2D_t *new = newRectangle(&node->rectangle->bottom_left->x, &node->rectangle->bottom_left->y, &mid_x, &mid_y);
        assert(new);
        node->quadrant[0] = createTree(new);
    }
    if (section == 1){
        //second quadrant. bottom left is the left centre while the top right is the top centre
        rectangle2D_t *new = newRectangle(&node->rectangle->bottom_left->x, &mid_y, &mid_x, &node->rectangle->top_right->y);
        assert(new);
        node->quadrant[1] = createTree(new);
    }
    if (section == 2) {
    //third quadrant, bottom left is centre and top right is original top right;
        rectangle2D_t *new = newRectangle(&mid_x, &mid_y, &node->rectangle->top_right->x, &node->rectangle->top_right->y);
        assert(new);
        node->quadrant[2] = createTree(new);
    }
    if (section == 3) {
            //fourth quadrant, bottom left is bottom centre and top right is right centre.
        rectangle2D_t *new = newRectangle(&mid_x, &node->rectangle->bottom_left->y, &node->rectangle->top_right->x, &mid_y);
        assert(new);
        node->quadrant[3] = createTree(new);
    }
}

void insertPoint(quadtreeNode_t *node, dataPoint_t *new_data) {
    int i, j, k;
    if (!inRectangle(new_data->location, node)) {
       return;
    }
    //node is empty, just set data and change colour to black
    if (node->colour == WHITE) {
        node->data = new_data;
        node->colour = BLACK;
      
    //black, already has a point
    } else if (node->colour == BLACK) {
        //check if the location is the same
        if ((new_data->location->x == node->data->location->x) && (new_data->location->y == node->data->location->y) ) {
            //append 1 element to the array
            sortedArrayInsert(node->data->arr_footpath, new_data->arr_footpath->A[0]);
            //arrayFree(new_data->arr_footpath);
            freeData(new_data);

        //creating a subtree adding the old node into the subtree
        } else {
            i = determineQuadrant(node->data->location, node);
            if (node->quadrant[i] == NULL) {
                divideTree(node, i);  
            }

            insertPoint(node->quadrant[i], node->data);
            node->data = NULL;
            //for new node
            j = determineQuadrant(new_data->location, node);
            if (node->quadrant[j] == NULL) {
                divideTree(node, j);  
            }
            insertPoint(node->quadrant[j], new_data);
            node->colour = GREY;
        }

    } else if (node->colour == GREY) {
        k = determineQuadrant(new_data->location, node);
        if (node->quadrant[k] == NULL) {
            divideTree(node, k);  
        }
        insertPoint(node->quadrant[k], new_data);
    }
}

void convertQuad(int i) {
    if (i == 0) {
        printf(" SW");
    } else if (i == 1) {
        printf(" NW");
    }else if (i == 2) {
        printf(" NE");
    } else if (i == 3) {
        printf(" SE");
    }
}
    
void print_attributes(FILE *f, footpath_t *path) {
    fprintf(f, "--> footpath_id: %d || address: %s || clue_sa: %s || asset_type: %s || deltaz: %lf || distance: %lf || grade1in: %lf || mcc_id: %d || \
     mccid_int: %d || rlmax: %lf || rlmin:%lf || segside: %s || statusid: %d || streetid: %d || street_group: %d || start_lat: %lf || start_lon: %lf || \
     end_lat: %lf ||  end_lon: %lf || \n", 
    path->footpath_id, path->address, path->clue_sa, path->asset_type, path->deltaz,  path->distance, path->grade1in, path->mcc_id, path->mccid_int, 
    path->rlmax, path->rlmin, path->segside, path->statusid, path->streetid, path->street_group, path->start_lat, path->start_lon, path->end_lat, path->end_lon);
    
}

int rectangleOverlap(rectangle2D_t *rect1, rectangle2D_t *rect2) {
    // If one rectangle is on left side of other
    if (rect1->top_right->x < rect2->bottom_left->x || rect2->top_right->x < rect1->bottom_left->x)
        return 0;
    // If one rectangle is above other
    if (rect1->top_right->y < rect2->bottom_left->y || rect2->top_right->y < rect1->bottom_left->y)
        return 0;
    return 1;
}

//traverses throught the quad tree recursively to find point and print out nodes that were visited
void searchPoint(quadtreeNode_t *node, point2D_t *point, FILE *f) {
    if (!inRectangle(point,node)) {
        return;
    }
    if (node->colour == BLACK) {
        if ((lfabs(node->data->location->x - point->x) < EPSILON) && (lfabs(node->data->location->y - point->y) < EPSILON)) { 
            fprintf(f, "%Lf %Lf\n", point->x, point->y);
            
            for (int i = 0; i < node->data->arr_footpath->n; i++) {
                print_attributes(f, node->data->arr_footpath->A[i]);
            }
        }
    }
    if (node->colour == GREY) {
        int i = determineQuadrant(point, node);
        convertQuad(i);
        searchPoint(node->quadrant[i], point, f);    
    } 
}
//rangeQuery: takes a 2D rectangle as argument and returns all datapoints in the PR quadtree 
//whose coordinates lie within the query rectangle.
void rangeQuery(quadtreeNode_t *node, rectangle2D_t *rectangle, array_t *unique, char *input_string){
    if (node->colour == 0) {
        return;
    }
    if (!rectangleOverlap(rectangle, node->rectangle)) {
        return;
    }
    if (node->colour == 1) {
        if (inRectangle2(node->data->location, rectangle)) {
            for (int j = 0; j < node->data->arr_footpath->n; j++) {
                uniqueInsert(unique, node->data->arr_footpath->A[j]);
            }
        }
    }
    
    if (node->colour == 2) {
        for (int i = 0; i < NUM_QUADS; i++) {
            if (node->quadrant[i] == NULL) {
                continue;
            }
            if (node->quadrant[i]->colour != 0) {
                if (rectangleOverlap(rectangle, node->quadrant[i]->rectangle)) {
                    convertQuad(i);
                }
                rangeQuery(node->quadrant[i], rectangle, unique, input_string);    
            }
        }
    }
}

//free datapoint structure
void freeData(dataPoint_t *data) {
    if (data == NULL) {
        return;
    }
    free(data->location);
    arrayFree(data->arr_footpath);
    free(data);
}

//free rectangle structure
void freeRectangle(rectangle2D_t *rectangle){
    if (rectangle == NULL) {
        return;
    }
    free(rectangle->bottom_left);
    free(rectangle->top_right);
    free(rectangle);

}

//recursively frees the nodes in the trees before freeing the tree itself
void freeTree(quadtreeNode_t *tree){
	if(tree == NULL){
        return;    
    }
    freeTree(tree->quadrant[0]);
    freeTree(tree->quadrant[1]);
    freeTree(tree->quadrant[2]);
    freeTree(tree->quadrant[3]);
    free(tree->quadrant);
    freeData(tree->data);
    freeRectangle(tree->rectangle);
    free(tree);
}



