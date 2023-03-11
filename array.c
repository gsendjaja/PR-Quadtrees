#include "array.h"
#include "structures.h"
#include "structures.c"

//creates space for array and returns the empty array
//taken from workshop 3.8
array_t *create_array() {
    array_t *arr = malloc(sizeof(*arr));
    assert(arr);
    int size = INIT_SIZE;
    arr->size = size;
	arr->A = malloc(size * sizeof(*(arr->A)));
	assert(arr->A);
	arr->n = 0;
	return arr;
}

//ensures that array size is equal to number of elements used
//taken from workshop 3.8
void arrayShrink(array_t *arr) {
	if (arr->size != arr->n) {
		// the new size should be at least INIT_SIZE
		arr->size = (arr->n > INIT_SIZE) ? arr->n : INIT_SIZE;
		arr->A = realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

//ensures that there is enough space in the array
//taken from workshop 3.8
void arrayEnsureSize(array_t *arr) {
	if (arr->n == arr->size) {
		arr->size <<= 1;       // same as arr->size *= 2;
		arr->A = realloc(arr->A, arr->size * sizeof(*(arr->A)));
		assert(arr->A);
	}
}

// inserts/appends node B to the end of array "arr" 
//taken from workshop 3.8
void arrayAppend(array_t *arr, footpath_t *B) {
	arrayEnsureSize(arr);
	arr->A[arr->n]= B;
	arr->n++;
}

//taken from workshop 3.8
int get_footpathID(footpath_t *path) {
    return path->footpath_id;
}
//takes 2 footpaths and returns an integer depending on which id is larger
int id_compare(footpath_t *a, footpath_t *b) {
	if (get_footpathID(a) < get_footpathID(b)) return -1;
	if (get_footpathID(a) > get_footpathID(b)) return +1;
	return 0;
}

//ensures that the array is sorted when inserting
//taken from workshop 3.8
void sortedArrayInsert(array_t *arr, footpath_t *B) {
	int i;
	arrayEnsureSize(arr);
	// starting from the end, shift all elements > s one position to the right
	for (i = arr->n - 1; i >=0 && id_compare(B, arr->A[i]) == -1; i--) {
		arr->A[i + 1] = arr->A[i];
	}
	// now "B" should be in A[i+1]
	arr->A[i + 1] = B;
	arr->n++;
}

//ensures that the footpath that is inserted is not a duplicate in the sorted array
void uniqueInsert(array_t *old, footpath_t *new) {
    int hasRepeat = 0;
    for (int i = 0; i < old->n; i++) {
        if (old->A[i] == new) {
            hasRepeat++;
        }
    }
    if (hasRepeat == 0) {
        sortedArrayInsert(old, new);
    }
}

//frees the memory the array was pointing to as well as the memory allocated to the array
void arrayFree(array_t *arr) {
	assert(arr != NULL);
	free(arr->A);
	free(arr);
}


