/**
@author ALARY Dorian
@brief Implementation of type Queue
@date 03 / 2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"


typedef struct s_element{
  void *value;
  struct s_element *next;
} Element;


struct s_queue{
  struct s_element *head;
  struct s_element *tail;
  int gauge;
};


/**
* @brief    Constructor : create and initialise Queue
* @return   Pointer to Queue
*/
Queue *createQueue(){

  Queue *q = malloc(sizeof(struct s_queue));
  q->head = NULL;
  q->tail = NULL;
  q->gauge = 0;
  return q;
}


/**
* @brief    Constructor : Push a new element in Queue
* @param q  Pointer on Queue
* @param e  Pointer on new element (whatever the type)
* @return   Pointer on Queue
*/
Queue *pushQueue(Queue *q, void *e){

  Element **insert_at = (q->gauge ? &(q->tail->next) : &(q->head));
  Element *new = malloc(sizeof(Element));
  new->value = e;
  new->next = NULL;
  *insert_at = q->tail = new;
  ++(q->gauge);
  return q;
}


/**
* @brief    Destructor : Delete queue
* @param q  Pointer on pointer on Queue
*/
void deleteQueue(Queue *q){

  while(q->gauge != 0){

    void *elem = topQueue(q);
    popQueue(q);
		free(elem);
	}
	free(q);

}


/**
* @brief    Operator : return if Queue is empty
* @param q  Pointer on Queue
* @return   Boolean
*/
bool isEmptyQueue(Queue *q){

  return (q->gauge == 0);
}


/**
* @brief    Operator : pop an element of Queue
* @param q  Pointer on Queue
* @pre      !isEmptyQueue(Queue *q)
*/
Queue *popQueue(Queue *q){
  assert(!isEmptyQueue(q));

  Element *pop = q->head;
  if (!(q->head = q->head->next))
    q->tail = q->head;
  q->gauge--;
  free(pop);
  return q;
}


/**
* @brief    Operator : return an element of Queue
* @param q  Pointer on Queue
* @pre      !isEmptyQueue(Queue *q)
*/
void *topQueue(Queue *q){
  return q->head->value;
}


/**
* @brief    Return size of Queue
* @param q  Pointer on Queue
* @return   int size of Queue
*/
int sizeQueue(Queue *q){
  return q->gauge;
}
