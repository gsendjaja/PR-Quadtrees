#include "data.h"
#include "array.h"
#include "structures.c"

//code taken from Workshop 3.8
int processArgs(int argc, char *argv[]) {
	if (argc < 7 || atoi(argv[1]) < STAGE_THREE || atoi(argv[1]) > STAGE_FOUR ) {
		exit(EXIT_FAILURE);
	}
	return atoi(argv[1]);
}

//function to read strings 
char *read_string(FILE *f) {
    char temp[MAX_FIELD_LEN + 1];
    temp[0] = getc(f);
    //if its a comma then the field is empty
    if(temp[0] == ',') {
        return strdup("\0");
    } 
    //if its a quotation mark scan until the next mark
    if(temp[0] == '"') {
        fscanf(f, " %[^\"]\",", temp);
    } else {
        fscanf(f, "%[^,],", temp + 1);
    }
    return strdup(temp);
}

//code taken from workshop 3.8
void studentSkipHeaderLine(FILE *f) {
	while (fgetc(f) != '\n');
}

//reading the csv file into footpath structs
footpath_t *read_path(FILE *f) {
    footpath_t *path = NULL;
    int footpath_id;
    double deltaz, distance, grade1in, mcc_id, mccid_int, rlmax, rlmin, statusid, streetid, street_group;
    double start_lat, start_lon, end_lat, end_lon;
    char *address;
    char *clue_sa;
    char *asset_type;
    char *segside;

    if (fscanf(f, "%d,", &footpath_id) == 1) {
        path = malloc(sizeof(*path));     // allocates memory for path
    }
    assert(path);
    path->footpath_id = footpath_id;

    address = read_string(f);
    assert(address);
    path->address = address;

    clue_sa = read_string(f);
    assert(clue_sa);
    path->clue_sa = clue_sa;

    asset_type = read_string(f);
    assert(asset_type);
    path->asset_type = asset_type;

    fscanf(f, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,", &deltaz, &distance
            ,&grade1in, &mcc_id, &mccid_int, &rlmax, &rlmin);
    path->deltaz = deltaz;
    path->distance = distance;
    path->grade1in = grade1in;
    path->mcc_id = (int) mcc_id;
    path->mccid_int = (int) mccid_int;
    path->rlmax = rlmax;
    path->rlmin = rlmin;
   
    segside = read_string(f);
    assert(segside);
    path->segside = segside;

    fscanf(f, "%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", &statusid, &streetid, &street_group
            , &start_lat, &start_lon, &end_lat, &end_lon);
    path->statusid = (int) statusid;
    path->streetid = (int) streetid;
    path->street_group = (int) street_group;
    path->start_lat = start_lat;
    path->start_lon = start_lon;
    path->end_lat = end_lat;
    path->end_lon = end_lon;
    return path;
}

//allocating memory to create an empty list
//taken from "listops.c" from Alistair's book
list_t
*make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

//inserting a new footpath record and appending to the end of the list
//taken from "listops.c" from Alistair's book
list_t
*insert_at_foot(list_t *list, footpath_t *record) {
    node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->path = record;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

//freeing the list
//taken from "listops.c" from Alistair's book
void listFree(list_t *list){
	assert(list != NULL);
	node_t *curr = list->head; // curr points to the first node
	while (curr) {             // while curr not reaching the end of list
		node_t *tmp = curr;
		curr = curr->next;     // advance curr to the next next node 
		free_data(tmp->path);             //freeing the space used by data
		free(tmp);
	}
	free(list);                // free the list itself
} 


//freeing all the memory allocated for strings when calling strdup
void free_data(void *vdata) {
    assert(vdata != NULL);
    footpath_t *data= vdata;
    free(data->address);
    free(data->clue_sa);
    free(data->asset_type);
    free(data->segside);
    free(data);
}
