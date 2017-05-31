/*
 * SPArrayList.c
 *
 *  Created on: May 25, 2017
 *      Author: sapir
 */

#include "SPArrayList.h"

/**
 *  Creates an empty array list with the specified maximum capacity.
 *  @param maxSize - the maximum capacity of the target array list.
 *  @return
 *  NULL, if an allocation error occurred or maxSize  <= 0.
 *  An instant of an array list otherwise.
 */
SPArrayList* spArrayListCreate(int maxSize)
{
	SPArrayList* array = (SPArrayList*) malloc(sizeof(SPArrayList));
	array->maxSize = maxSize;
	array->elements = (int*) malloc(sizeof(int) * maxSize);
	array->actualSize = 0;
	return array;
}

/**
 *	Creates an exact copy of the src array list. Elements in the new copy will
 *	be in the same order as they appeared in the source list.
 *	@param src - the source array list.
 *	@return
 *	NULL if either an allocation error occurs or src == NULL.
 *	A new copy of the source array list, otherwise.
 */
SPArrayList* spArrayListCopy(SPArrayList* src)
{
	if(src == NULL){ return NULL; }
	SPArrayList* dst = (SPArrayList*) malloc(sizeof(SPArrayList));
	dst->actualSize = src->actualSize;
	dst->maxSize = src->maxSize;
	int *dst_elements = (int*) malloc(sizeof(int) * src->maxSize);
	if(dst_elements == NULL){ return NULL;}
	memcpy(dst_elements, src->elements, (sizeof(int) * src->maxSize));
	dst->elements = dst_elements;
	return dst;
}

/**
 * Frees all memory resources associated with the source array list. If the
 * source array is NULL, then the function does nothing.
 * @param src - the source array list
 */
void spArrayListDestroy(SPArrayList* src)
{
	if(src != NULL){
		free(src->elements);
		free(src);
	}
}

/**
 * Clears all elements from the source array list. After invoking this function,
 * the size of the source list will be reduced to zero and maximum capacity is
 * not affected.
 * @param src - the source array list
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT if src == NULL
 * SP_ARRAY_LIST_SUCCESS otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	free(src->elements);
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a specified index. The elements residing at and after the
 * specified index will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @param index - the index where the new element will be placed. The index is
 *                0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == src->maxSize){
		return SP_ARRAY_LIST_FULL;
	}
	if((src->actualSize) < index){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}

	int one_before = *((src->elements) + index);
	*((src->elements) + index) = elem;
	int temp = 0;

	for (int i = index + 1; i < (src->maxSize); i++){
		temp = *((src->elements) + i);
		*((src->elements) + i) = one_before;
		one_before = temp;
	}
	(src->actualSize)++;
	return SP_ARRAY_LIST_SUCCESS;
}

/**
 * Inserts element at a the beginning of the source element. The elements
 * will be shifted to make place for the new element. If the
 * array list reached its maximum capacity and error message is returned and
 * the source list is not affected
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
 SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem){
	 return spArrayListAddAt(src, elem, 0);
 }

/**
 * Inserts element at a the end of the source element. If the array list
 * reached its maximum capacity and error message is returned and the source
 * list is not affected.
 * @param src   - the source array list
 * @param elem  - the new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_FULL - if the source array list reached its maximum capacity
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem){
	return spArrayListAddAt(src, elem, src->actualSize);
}

/**
 * Removes an element from a specified index. The elements residing after the
 * specified index will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @param index - The index from where the new element will be removed.
 *                The index is 0-based.
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL or the index is out of bound
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if(src == NULL){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if(src->actualSize == 0){
			return SP_ARRAY_LIST_EMPTY;
	}
	if(index + 1 > (src->actualSize)){
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	for(int i = index; i <= (src->actualSize) - 2; i++){
		*((src->elements) + i) = *((src->elements) + i + 1);
	}
	*((src->elements) + src->actualSize - 1) = 0;
	(src->actualSize)--;
	return SP_ARRAY_LIST_SUCCESS;

}

/**
 * Removes an element from a the beginning of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

/**
 * Removes an element from a the end of the list.
 * The elements will be shifted to make to keep the list continuous. If the
 * array list is empty then an error message is returned and the source list
 * is not affected
 * @param src   - The source array list
 * @param elem  - The new element to be inserted
 * @return
 * SP_ARRAY_LIST_INVALID_ARGUMENT - if src == NULL
 * SP_ARRAY_LIST_EMPTY - if the source array list is empty
 * SP_ARRAY_LIST_SUCCESS - otherwise.
 */
SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src, (src->actualSize) - 1);
}

/**
 * Returns the element at the specified index. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @param index - the specified index, the index is 0-based.
 * @return
 * Undefined value if either src == NULL or index out of bound.
 * Otherwise, the element at the specified index is returned.
 */
int spArrayListGetAt(SPArrayList* src, int index){
	return *(src->elements + index);
}

/**
 * Returns the element at the beginning of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the beginning of the list is returned.
 */
int spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt(src, 0);
}

/**
 * Returns the element at the end of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL or the list is empty
 * Otherwise, the element at the end of the list is returned.
 */
int spArrayListGetLast(SPArrayList* src){
	return spArrayListGetAt(src, (src->actualSize) - 1);
}

/**
 * Returns the maximum capacity of the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the maximum capacity of the list is returned.
 */
int spArrayListMaxCapacity(SPArrayList* src){
	return src->maxSize;
}

/**
 * Returns the number of elements in the list. The function is called
 * with the assertion that all arguments are valid. If any of the arguments is
 * invalid then an undefined value is returned.
 * @param src - the source array list
 * @return
 * Undefined value if either src == NULL
 * Otherwise, the number of the elements in the list is returned.
 */
int spArrayListSize(SPArrayList* src){
	return src->actualSize;
}

/**
 * Returns true if the list is full, that is the number of elements in the list
 * equals its maximum capacity.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is less
 * than its maximum capacity.
 * Otherwise, true is returned.
 */
bool spArrayListIsFull(SPArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == src->maxSize){
		return true;
	}
	return false;
}

/**
 * Returns true if the list is empty, that is the number of elements in the list
 * equals to zero.
 * @param src - the source array list
 * @return
 * false if either src == NULL or the number of elements in the list is not zero.
 * Otherwise, true is returned.
 */
bool spArrayListIsEmpty(SPArrayList* src){
	if(src == NULL){
		return false;
	}
	if(src->actualSize == 0){
		return true;
	}
	return false;
}

/*
 * push element at first, if the src is full:
 * remove the last element and then add at first
 * **/

SP_ARRAY_LIST_MESSAGE spArrayListPushFirst(SPArrayList* src, int elem){
	if(spArrayListIsFull(src)){
		SP_ARRAY_LIST_MESSAGE status = spArrayListRemoveLast(src);
		if(status != SP_ARRAY_LIST_SUCCESS){
			return status;
		}
		status = spArrayListAddFirst(src, elem);
		return status;
	}
	else{
		return spArrayListAddFirst(src, elem);
	}
}

/**
 * print the list
 * */
void spArrayListPrint(SPArrayList* src){
	for(int i=0; i<src->actualSize; i++){
		if(i<src->actualSize-1){
			printf("%d, ", *((src->elements) + i));
			fflush(stdout);
		}
		else{
			printf("%d", *((src->elements) + i));
			fflush(stdout);
		}
	}
	printf("\n");
	fflush(stdout);
}
