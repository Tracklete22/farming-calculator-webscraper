/* counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * Myles Holt, 18 July 2018
 */

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h> 
#include "counters.h"
#include "memory.h"


/**************** file-local global variables ****************/
/* none */

/**************** global types ****************/
typedef struct counterset {
  struct counternode *head;
}counters_t;

/**************** loval types ****************/
typedef struct counternode {
  int count;
  int key;
  struct counternode *next;
}counter_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */


/* that is, visible outside this file */
/* see bag.h for comments about exported functions */


/**************** counters_new function ****************/
  counters_t *counters_new(void) 
  {
  counters_t *counter = count_malloc(sizeof(counter)); //Create a new counter
  if (counter == NULL) {
    return NULL;
  } else {
    counter->head = NULL;
    return counter;
  }
}

/**************** counters_add function ****************/
int counters_add(counters_t *ctrs,
  const int key) 
{
  if (key >= 0 && ctrs != NULL) { //Key must be greater than 0 to be added
    if (counters_get(ctrs, key) == 0) { // If key not found in counters, create a new counter node
      counter_t *new = count_malloc(sizeof(counter_t));

      new->count = 1; //Initialize to 1
      new->key = key;
      new->next = ctrs->head;
      ctrs->head = new; // Place new node to the front of the linked list
    } else {
      counters_set(ctrs, key, counters_get(ctrs, key) + 1); // Iif key already in list, increment the count by 1
    }
  }
  return counters_get(ctrs, key);
}

/**************** counters_get function ****************/
int counters_get(counters_t *ctrs,
  const int key) 
{

  if (ctrs != NULL) {
    for (counter_t *ctrnode = ctrs->head; ctrnode != NULL; ctrnode = ctrnode->next) {
      if (key == ctrnode->key) { //If the key given equals the node's key (i.e it is found), return that node's count
        return ctrnode->count;
      }
    }
  }
  return 0;
}
/**************** counters_set function ****************/
void counters_set(counters_t *ctrs,
  const int key, int count) 
{
  if (ctrs == NULL || key < 0 || count < 0) {;
  }

  if (counters_get(ctrs, key) == 0) { //If key is not in the list, create a new counternode
    counter_t *new = count_malloc(sizeof(counter_t));
    new->count = count; //set node's count to be equal to count
    new->next = ctrs->head;
    new = ctrs->head; //Place node to be at the head (front) of linked list
  } else {
    for (counter_t *ctrnode = ctrs->head; ctrnode != NULL; ctrnode = ctrnode->next) {
      if (ctrnode->key == key) { //If key is found, change it's count to the count given by the user
        ctrnode->count = count;
        break;
      }
    }
  }
}

/**************** counters_print function ****************/
void counters_print(counters_t *ctrs, FILE *fp) 
{
  if (ctrs == NULL) {
    fputs("(null)", fp);
  }

  if (fp == NULL) {;
  } else {
    fputc('{', fp); //represents beginning of the counter
    for (counter_t *ctrnode = ctrs->head; ctrnode != NULL; ctrnode = ctrnode->next) {
      fprintf(fp, "[%d:%d], ", ctrnode->key, ctrnode->count); // prints each node in "[key:count] format"
    }
    fputc('}', fp); //Finally, close the counter
  }

}

/**************** counters_iterate function ****************/
void counters_iterate(counters_t *ctrs, void *arg, void( *itemfunc)(void *arg,
  const int key, int count)) 
{
  if (ctrs == NULL || *itemfunc == NULL) {;
  } else {
    for (counter_t *ctrnode = ctrs->head; ctrnode != NULL; ctrnode = ctrnode->next) {
      ( *itemfunc)(arg, ctrnode->key, ctrnode->count); //Iterate through each node and perform the "itemfunc" function on that node. This
                                                       // itemfunc function is given by the user in a separate test file
    }
  }
}

/**************** counters_delete function ****************/
void counters_delete(counters_t *ctrs) 
{
  if (ctrs != NULL) {
    for (counter_t *ctrnode = ctrs-> head; ctrnode != NULL; ctrnode = ctrnode->next) { 
      counter_t *next = ctrnode->next; //Keep a temporary variable so that the reference to the next node isn't lost
      count_free(ctrnode); // free the node
      ctrnode = next;
    }

  }
  count_free(ctrs);
}
