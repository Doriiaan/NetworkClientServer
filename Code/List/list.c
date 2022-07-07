/**
 * @file list.c
 * @author Alary Dorian
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

typedef struct s_LinkedElement {

	void *value;
	struct s_LinkedElement *previous;
	struct s_LinkedElement *next;
} LinkedElement;


struct s_List {
	
	LinkedElement *sentinel;
	int size;
};

/*-----------------------------------------------------------------*/

/** 
* @brief	Specification of the the constructor
* @return	List
*/
List *list_create() 
{
	List *l = malloc(sizeof(List));
  	l->sentinel = malloc(sizeof(LinkedElement));
  	l->sentinel->previous = l->sentinel->next = l->sentinel;
  	l->size = 0;

	return l;
}


/** Specification of the the constructor \c push_back
* @brief 	Add the value v at the end of the list l.
* @param l 	The list to modify
* @param v 	The value to add
* @return 	The modified list
* @note 	This function acts by side effect on the parameter l. 
			The returned value is the same as the parameter l that is modified by the function.
*/
List *list_push_back(List *l, void *v) 
{
	LinkedElement* e = malloc(sizeof(LinkedElement));
	e->value = v;
	e->next = l->sentinel;
	e->previous = l->sentinel->previous;
	e->previous->next = e;
	l->sentinel->previous = e;
	l->size++;
	return l;
}


/** Destructor.
* @brief 	Free ressources allocated by constructors.
* @param l 	The adress of the list.
* @note 	After calling this function, the list l becomes NULL.
 */
void list_delete(List *l) 
{

	while(l->size != 0)
	{
		l->sentinel->next = l->sentinel->next->next;
		free(l->sentinel->next->previous);
		l->sentinel->next->previous = l->sentinel;
		l->size--;
	}
	free(l->sentinel);
	free(l);
}


/** 
* @brief 	Add an element at the front of the list.
* @param l 	The list to modify
* @param v 	The value to add (void * type)
* @return 	The modified list
* @note 	The returned value is the same as the parameter l that is modified by the function.
 */
List *list_push_front(List *l, void *v) 
{
	LinkedElement* e = malloc(sizeof(LinkedElement));
	e->value = v;
	e->previous = l->sentinel;
	e->next = l->sentinel->next;
	e->next->previous=e;
	l->sentinel->next = e;
	l->size++;

	return l;
}


/** 
* @brief 	Acces to the element at begining of the list.
* @return 	The value of the front element of l.
* @pre 		!empty(l)
*/
void *list_front(List *l) 
{
	return l->sentinel->next->value;
}


/** 
* @brief 	Acces to the element at end of the list.
* @return 	The value of the back element of l.
* @pre 		!empty(l)
*/
void *list_back(List *l) 
{
	return l->sentinel->previous->value;
}


/** 
* @brief 	Remove the element at begining of the list.
* @return 	The modified list
* @note		This function acts by side effect on the parameter l. The returned value is the same as the parameter l that is modified by the function.
* @pre 		!empty(l)
*/
List *list_pop_front(List *l)
{
	assert(!list_is_empty(l));

	LinkedElement *pop_elem = l->sentinel->next;
	l->sentinel->next = pop_elem->next;
	pop_elem->next->previous = l->sentinel;
	l->size--;
	free(pop_elem);

	return l;
}


/** 
* @brief 	Remove the element at end of the list.
* @return 	The modified list
* @note 	This function acts by side effect on the parameter l. The returned value is the same as the parameter l that is modified by the function.
* @pre 		!empty(l)
*/
List *list_pop_back(List *l)
{
	assert(!list_is_empty(l));

	LinkedElement *pop_elem = l->sentinel->previous;
	l->sentinel->previous = pop_elem->previous;
	pop_elem->previous->next = l->sentinel;
	l->size--;
	free(pop_elem);

	return l;
}


/** 
* @brief 	Insert an element at a given position.
* @param l 	The list to modify.
* @param p 	The position to insert.
* @param v 	The value to add.
* @return	The modified list.
* @pre 		0 <= p <= list_size(l)
* @note 	This function acts by side effect on the parameter l. 
			The returned value is the same as the parameter l that is modified by the function.
*/
List *list_insert_at(List *l, int p, void *v) 
{

	LinkedElement *new = malloc(sizeof(LinkedElement));
	new->value = v;

	LinkedElement *prev = l->sentinel;
		for (int i = 0; i < p; i++) 
		{
		prev = prev->next;
	}
	new->previous = prev;
	new->next = prev->next;
	prev->next = new;
	new->next->previous = new;
	l->size++;
	return l;
}


/** 
* @brief 	Remove an element at a given position.
* @param l 	The list to modify.
* @param p 	The position of the element to be removed.
* @return 	The modified list.
* @pre 		0 <= p < list_size(l)
* @note 	This function acts by side effect on the parameter l.
 			The returned value is the same as the parameter l that is modified by the function.
*/
List *list_remove_at(List *l, int p) 
{

	LinkedElement *supp = l->sentinel->next;
	for (int i = 0; i < p; i++) 
	{
		supp = supp->next;
	}
	supp->previous->next = supp->next;
	supp->next->previous = supp->previous;
	free(supp);
	l->size--;
	return l;
}


/**
* @brief 	Acces to an element at a given position.
* @param l 	The list to acces.
* @param p 	The position to acces.
* @return 	The value of the element at position p.
* @pre 		0 <= p < list_size(l)
*/
void *list_at(List *l, int p)
{

	LinkedElement *now = l->sentinel->next;
	for (int i = 0; i < p; i++) 
	{
		now = now->next;
	}
	return now->value;
}


/** 
* @brief 	Test if a list is empty.
*/
bool list_is_empty(List *l) 
{
	return l->size == 0;
}


/** 
* @brief 	Give the number of elements of the list.
*/
int list_size(List *l) 
{
	return l->size;
}


/** 
* @brief 	Apply the same operator on each element of the list.
* @param l 	The list to process.
* @param f 	The operator (function) to apply to each element
* @see 		SimpleFunctor
* @return 	The eventually modified list
* @note 	If the elements are modified by the operator f, this function acts by side effect on the parameter l. 
			The returned value is the same as the parameter l that is modified bye the function.
 			This function sequentially apply the operator f on each element of the list, 
			starting from the beginning of the list until the end. 
			The value reurned by the operator when called on an element will replace the element.
*/
List * list_map(List *l, SimpleFunctor f) 
{

	for (LinkedElement* e = l->sentinel->next; e != l->sentinel; e=e->next) 
	{
    	e->value = f(e->value);
	}

	return l;
}


/** 
* @brief 	Apply the same operator on each element of the list gieven a user define environment.
* @param l 	The list to process.
* @param f 	The operator (function) to apply to each element
* @param userData The environment used to call the operator f together with each list element.
* @see 		ReduceFunctor
* @return 	The eventually modified list
* @note 	If the elements are modified by the operator f, this function acts by side effect on the parameter l. 
			The returned value is the same as the parameter l that is modified bye the function.
 			This function sequentially apply the operator f on each element of the list with the user supplied environment defined by the abstract pointer userData. 
			The operator is applied starting from the beginning of the list until the end. 
			The value reurned by the operator when called on an element will replace the element.
*/
List *list_reduce(List *l, ReduceFunctor f, void *userData) 
{
	
	for (LinkedElement* e = l->sentinel->next; e != l->sentinel; e=e->next) 
	{
		f(e->value, userData);
	}

	return l;
}

/*-----------------------------------------------------------------*/

/* 	If a value are remove in the list during a loop on the list with the iterator, program crash.
	The solution is to begin a loop, remove a value, break the loop, and begin a new loop.
*/

struct s_ListIterator {
	
	List *collection; /* the collection the iterator is attached to */
	LinkedElement *current; /* the current element pointed by the iterator */
	LinkedElement *begin; /* the first element according to the iterator direction */
	LinkedElement * (*next)(LinkedElement *); /* function that goes to the next element according to the direction */
};

/**
 * @brief next function for forward iterator
 * 
 * @param e element of collection
 * @return next element of e
 */
LinkedElement *goto_next(LinkedElement *e) 
{
	return e->next;
}


/**
 * @brief previous function for forward iterator
 * 
 * @param e element of collection
 * @return previous element of e
 */
LinkedElement *goto_previous(LinkedElement *e) 
{
	return e->previous;
}


/**
 * @brief create an iterator for a collection
 * 
 * @param collection list
 * @param direction reffered to define FORWARD_ITERATOR and BACKWARD_ITERATOR, tail to head and head to tail
 * @return ListIterator* pointer on iterator
 */
ListIterator *listIterator_create(List *collection, int direction){

	ListIterator *it = malloc(sizeof(ListIterator));
	it->collection = collection;
	
	if (direction == FORWARD_ITERATOR) 
	{
		it->begin = collection->sentinel->next;
		it->next = goto_next;
	} 
	else 
	{
		it->begin = collection->sentinel->previous;
		it->next = goto_previous;
	}

	it->current = it->begin;
	return it;
}


/**
 * @brief set the value of iterator on the begin of list
 * 
 * @param i pointer on iterator
 * @return ListIterator* pointer on iterator
 */
ListIterator *listIterator_begin(ListIterator *it) 
{ 
	it->begin = it->next(it->collection->sentinel);
	it->current = it->begin;
	return it;
}


/**
 * @brief look if the value of iterator is the end of list
 * 
 * @param i pointer on iterator
 * @return true iterator is the end of list
 * @return false iterator is not the end of list
 */
bool listIterator_end(ListIterator *it) 
{
	return it->current == it->collection->sentinel;
}


/**
 * @brief set iterator on the next element of the list
 * 
 * @param i pointer on iterator
 * @return ListIterator* pointer on iterator
 */
ListIterator *listIterator_next(ListIterator *it) 
{
	it->current = it->next(it->current);
	return it;
}


/**
 * @brief return the value the iterator
 * 
 * @param i pointer on iterator
 * @return void * pointer on the current element
 */
void *listIterator_value(ListIterator *it) 
{
	return it->current->value;
}

/**
 * @brief Delete the iterator
 * 
 * @param it pointer on iterator
 */
void listIterator_delete(ListIterator *it)
{
	free(it);
}