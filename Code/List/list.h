/**
 * @file list.h
 * @author Alary Dorian
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>

/*-----------------------------------------------------------------*/


/** 
* @brief 	Opaque definition of type List.
*/
typedef struct s_List List;


/** 
* @brief 	Définition of type ptrList : pointer to a List.
*/
typedef List * ptrList;


/*-----------------------------------------------------------------*/


/** 
* @brief 	Functor to be used with the list_map operator.
* @param	(void*) Value of list 
* @return 	(void*) Value of list modified
*/
typedef void *(*SimpleFunctor)(void *);


/** 
* @brief 	Functor with user data to be used with the list_reduce operator.
* @param	(void*) Value of list 
* @param	(void*) Opaque pointer to user provided data
*/
typedef void(*ReduceFunctor)(void *, void *);


/*-----------------------------------------------------------------*/


/** 
* @brief	Specification of the the constructor
* @return	List
*/
List *list_create();


/** Specification of the the constructor
* @brief 	Add the value v at the end of the list l.
* @param l 	The list to modify
* @param v 	The value to add
* @return 	The modified list
* @note 	This function acts by side effect on the parameter l. 
			The returned value is the same as the parameter l that is modified by the function.
*/
List *list_push_back(List *l, void *v);


/** Destructor.
* @brief 	Free ressources allocated by constructors.
* @param l 	The adress of the list.
* @note 	After calling this function, the list l becomes NULL.
 */
void list_delete(List *l);


/** 
* @brief 	Add an element at the front of the list.
* @param l 	The list to modify
* @param v 	The value to add (void * type)
* @return 	The modified list
* @note 	The returned value is the same as the parameter l that is modified by the function.
 */
List *list_push_front(List *l, void *v);


/** 
* @brief 	Acces to the element at begining of the list.
* @return 	The value of the front element of l.
* @pre 		!empty(l)
*/
void *list_front(List *l);


/** 
* @brief 	Acces to the element at end of the list.
* @return 	The value of the back element of l.
* @pre 		!empty(l)
*/
void *list_back(List *l);


/** 
* @brief 	Remove the element at begining of the list.
* @return 	The modified list
* @note		This function acts by side effect on the parameter l. The returned value is the same as the parameter l that is modified by the function.
* @pre 		!empty(l)
*/
List *list_pop_front(List *l);


/** 
* @brief 	Remove the element at end of the list.
* @return 	The modified list
* @note 	This function acts by side effect on the parameter l. The returned value is the same as the parameter l that is modified by the function.
* @pre 		!empty(l)
*/
List *list_pop_back(List *l);


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
List *list_insert_at(List *l, int p, void *v);


/** 
* @brief 	Remove an element at a given position.
* @param l 	The list to modify.
* @param p 	The position of the element to be removed.
* @return 	The modified list.
* @pre 		0 <= p < list_size(l)
* @note 	This function acts by side effect on the parameter l.
 			The returned value is the same as the parameter l that is modified by the function.
*/
List *list_remove_at(List *l, int p);


/**
* @brief 	Acces to an element at a given position.
* @param l 	The list to acces.
* @param p 	The position to acces.
* @return 	The value of the element at position p.
* @pre 		0 <= p < list_size(l)
*/
void *list_at(List *l, int p);


/** 
* @brief 	Test if a list is empty.
*/
bool list_is_empty(List *l);


/** 
* @brief 	Give the number of elements of the list.
*/
int list_size(List *l);


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
List * list_map(List *l, SimpleFunctor f);


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
List *list_reduce(List *l, ReduceFunctor f, void *userData);


/*-----------------------------------------------------------------*/


/* Constant that control the list iterator behavior */
#define FORWARD_ITERATOR 1
#define BACKWARD_ITERATOR 0


/* Abstract data type */
typedef struct s_ListIterator ListIterator;


/**
 * @brief create an iterator for a collection
 * 
 * @param collection list
 * @param direction reffered to define FORWARD_ITERATOR and BACKWARD_ITERATOR, tail to head and head to tail
 * @return ListIterator* pointer on iterator
 */
ListIterator *listIterator_create(List *collection, int direction);


/**
 * @brief set the value of iterator on the begin of list
 * 
 * @param i pointer on iterator
 * @return ListIterator* pointer on iterator
 */
ListIterator *listIterator_begin(ListIterator *it);


/**
 * @brief look if the value of iterator is the end of list
 * 
 * @param i pointer on iterator
 * @return true iterator is the end of list
 * @return false iterator is not the end of list
 */
bool listIterator_end(ListIterator *it);


/**
 * @brief set iterator on the next element of the list
 * 
 * @param i pointer on iterator
 * @return ListIterator* pointer on iterator
 */
ListIterator *listIterator_next(ListIterator *it);


/**
 * @brief return the value the iterator
 * 
 * @param i pointer on iterator
 * @return void * pointer on the current element
 */
void *listIterator_value(ListIterator *it);


/**
 * @brief Delete the iterator
 * 
 * @param it pointer on iterator
 */
void listIterator_delete(ListIterator *it);

#endif

