#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "structures.h"
#include "structures.c"
#include "data.h"
#include "quad.h"
#include "array.h"

//Thank you so much to Dr Anh Vo and Grady for helping me implement certain
//functions as well as resolving my memory leaks
int main(int argc, char *argv[]) {
    footpath_t *paths;
    long double bottom_left_x, bottom_left_y, top_right_x, top_right_y;
    bottom_left_x = bottom_left_y = top_right_x = top_right_y = 0.0;
    //int stage_type;
    FILE *infile = fopen(argv[2], "r");
    FILE *outfile = fopen(argv[3], "w");
    assert(infile);
    assert(outfile);
    studentSkipHeaderLine(infile);
    int stage_type;
    char *ptr;
    //taking arguments as coordinates for the rectangle
    bottom_left_x = strtold(argv[4], &ptr);
    bottom_left_y = strtold(argv[5], &ptr);
    top_right_x = strtold(argv[6], &ptr);
    top_right_y = strtold(argv[7], &ptr);
    
    //creating rectangle for the new quadtreeNode
    rectangle2D_t *original = newRectangle(&bottom_left_x, &bottom_left_y, &top_right_x, &top_right_y);
    quadtreeNode_t *test = createTree(original);
    //linked list to keep of footpaths to free later
    list_t *footList = make_empty_list();

    while (!feof(infile)) {
         //reading the file and storing into footpath structs
        paths = read_path(infile);
        //inserting and sorting into tree
        buildTree(test, paths, footList);    
    }
    stage_type = processArgs(argc, argv);
        if (stage_type == STAGE_THREE) {
            char *input_string = NULL;
            point2D_t *input_point;
            long double x,y;
            size_t line_buffer_len = 0;
            int chars = 0;

            //read the queryfile
            while((chars = getline(&input_string, &line_buffer_len, stdin)) > 0) {
                if (input_string[chars-1] == '\n') {
                    input_string[chars-1] = '\0';
                }
                sscanf(input_string, "%Lf %Lf", &x, &y);
                input_point = newPoint(&x, &y);
                printf("%s -->", input_string);
                searchPoint(test, input_point, outfile);
                printf("\n");
                fflush(stdout);
                free(input_point);
            }
            fflush(stdout);
            free(input_string);
        
        } else if (stage_type == STAGE_FOUR) {
            char *input_string = NULL;
            size_t line_buffer_len = 0;
            int chars = 0;
            long double bl_x, bl_y, tr_x, tr_y;
            rectangle2D_t *new;
            //final array that will contain all unique footpahts
            array_t *final_array;
            //read the queryfile
            while((chars = getline(&input_string, &line_buffer_len, stdin)) > 0) {
                final_array = create_array();
                if (input_string[chars-1] == '\n') {
                    input_string[chars-1] = '\0';
                }
                printf("%s -->", input_string);
                sscanf(input_string, "%Lf %Lf %Lf %Lf", &bl_x, &bl_y, &tr_x, &tr_y);
                new = newRectangle(&bl_x, &bl_y, &tr_x, &tr_y);
                rangeQuery(test, new, final_array, input_string);
                printf("\n");
                fprintf(outfile, "%s\n", input_string);
                for (int i = 0; i < final_array->n; i++) {
                    print_attributes(outfile, final_array->A[i]);   
                }
                fflush(stdout);
                arrayFree(final_array);
                freeRectangle(new); 
            }
            fflush(stdout);  
            free(input_string);   
        }   
    freeTree(test);
    listFree(footList); 
    fclose(infile);
    fclose(outfile);
}



