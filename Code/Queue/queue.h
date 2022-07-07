/**
@author ALARY Dorian
@brief Interface of type Queue
@date 03 / 2022
*/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

/** Opaque definition of type Queue */
typedef struct s_queue Queue;


/**
* @brief    Constructor : create and initialise Queue
* @return   Pointer to Queue
*/
Queue *createQueue();


/**
* @brief    Constructor : Push a new element in Queue
* @param q  Pointer on Queue
* @param e  Pointer on new element (whatever the type)
* @return   Pointer on Queue
*/
Queue *pushQueue(Queue *q, void *e);


/**
* @brief    Destructor : Delete queue
* @param q  Pointer on Queue
*/
void deleteQueue(Queue *q);


/**
* @brief    Operator : return if Queue is empty
* @param q  Pointer on Queue
* @return   Boolean
*/
bool isEmptyQueue(Queue *q);


/**
* @brief    Operator : pop an element of Queue
* @param q  Pointer on Queue
* @pre      !isEmptyQueue(Queue *q)
*/
Queue *popQueue(Queue *q);


/**
* @brief    Operator : return an element of Queue
* @param q  Pointer on Queue
* @pre      !isEmptyQueue(Queue *q)
*/
void *topQueue(Queue *q);


/**
* @brief    Return size of Queue
* @param q  Pointer on Queue
* @return   int size of Queue
*/
int sizeQueue(Queue *q);

#endif
