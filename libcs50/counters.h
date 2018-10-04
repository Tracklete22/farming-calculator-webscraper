 /*
  * counters.h - header file for 'counters' module 
  *
  * A bag is an unordered collection of (items). The bag starts empty,
  * grows as the caller adds one item at a time, and shrinks as the caller
  * extracts one item at a time. It could be empty, or could contain hundreds of
  * items. Items are indistinguishable, so the extract function is free to return
  * any item from the bag. 
  * Myles Holt, 18 July 2018
  */

#ifndef __COUNTERS_H
#define __COUNTERS_H

#include <stdio.h>

/**************** global types ****************/
typedef struct counterset counters_t;  // opaque to users of the module


/**************** functions ****************/

/* Create a new (empty) counter structure; return NULL if error. */
counters_t *counters_new(void);

/* Increment the counter indicated by key; key must be >= 0.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * Ignore if ctrs is NULL or key is negative.
 * Return the new value of the counter related to the inserted key
 */
int counters_add(counters_t *ctrs, const int key);

/* Return current value of counter associated with the given key;
 * return 0 if ctrs is NULL or if key is not found.
 */
int counters_get(counters_t *ctrs, const int key);

/* Set the current value of counter associated with the given key;
 * If the key does not yet exist, create a counter for it and initialize to
 * the given value. Ignore if ctrs is NULL, if key < 0, or count < 0.
 */
void counters_set(counters_t *ctrs, const int key, int count);

/* Print all counters; provide the output file.
 * Ignore if NULL fp. Print (null) if NULL ctrs.
 */
void counters_print(counters_t *ctrs, FILE *fp);

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 * If ctrs==NULL or itemfunc==NULL, do nothing.
 */
void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count));

/* Delete the whole counters. ignore NULL ctrs. */
void counters_delete(counters_t *ctrs);

#endif // __COUNTERS_H
