/*
   hashtable.c - Code for the CS50 'hashtable' module
   Myles Holt
   18 July 2018
                */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include "hashtable.h"
#include "set.h"
#include "memory.h"
#include "jhash.c"


/**************** file-local global variables ****************/
/* none */


/**************** local functions ****************/

typedef struct hashtable {
  set_t ** set;
  int num_slots;
} hashtable_t;


/***************** functions ******************/

/***************** hashtable_new ******************/
hashtable_t *hashtable_new(const int num_slots) 
{
  hashtable_t *ht = count_malloc(sizeof(hashtable_t));
  if (ht == NULL) {
    return NULL;
  } else {
    set_t **set = count_calloc(num_slots, sizeof(set_t* )); //Create an array of pointers to sets
    if (set == NULL) {
      return NULL;
    } else {
      int i = 0;
      while (i < num_slots) { //Initialize each of the hashtable's slots to NULL
        set[i] = NULL;
        i++;
      }
      ht-> set = set;
      ht-> num_slots = num_slots;
      return ht;
    }
  }
}

/***************** hashtable_insert ******************/
 bool hashtable_insert(hashtable_t *ht,
  const char *key, void *item)
{
  if (ht == NULL || key == NULL || item == NULL) {
    return false;
}
  char *copy = malloc(strlen(key) + 1); //Allocate memory for a string that is the saeme size as key
  if (copy == NULL) {
    count_free(copy); //Free up that space if keycopy is NULL
    return false;

  } else {
    strcpy(copy, key); //copy key into keycopy
  }
  unsigned long index = JenkinsHash(copy, ht->num_slots); //create an index using the JenkinsHash hash function
  if (hashtable_find(ht,copy) != NULL) { //check to see whether or not the key is already in the hashtable
    count_free(copy);
    return false;
  } else {

    if (ht->set[index] != NULL) { //If nothing is occupied in the given slot, create a new set

       if (set_insert(ht->set[index], copy, item)) { //Put the set at the hashed index
      } else {
        return false;
      }

    } else {
     set_t *set = set_new();
      if (set == NULL) {
        return false;
      } else {
        if (!set_insert(set, copy, item)) { //return false if failed to insert the key
          return false;
        } else {
          ht->set[index] = set;
        }
      }
    }
  }

  count_free(copy);
  return true;
}

/***************** hashtable_find ******************/
void *hashtable_find(hashtable_t *ht,
  const char *key) 
{
  if (ht == NULL || key == NULL) {
    return NULL;
  }

  unsigned long index = JenkinsHash(key, ht-> num_slots); //Calculate key's hash index

  return set_find(ht-> set[index], key); //Use set_find to look through the sets at that index and return it
}

/***************** hashtable_print ******************/
void hashtable_print(hashtable_t *ht, FILE *fp, void( *itemprint)(FILE *fp,
  const char *key, void *item)) 
{
  if (fp == NULL) {;
  }
  if (fp != NULL) {
    if (ht == NULL) {
      fputs("(null)", fp);
    } else {
      fputc('[', fp); //To start the hashtable 

      if (itemprint != NULL) {
        int i = 0;
        while (i < ht-> num_slots) { // Print each of the items in the hashtable using itemprint
          set_print(ht-> set[i], fp, itemprint);
          i++;
        }
      }
      fputc(']', fp); //Once finished, close hashtable
    }
  }
}

/***************** hashtable_iterate ******************/
void hashtable_iterate(hashtable_t *ht, void *arg, void( *itemfunc)(void *arg,
  const char *key, void *item)) 
{
  if (ht == NULL || itemfunc == NULL) {;
  } else {

    for (int i = 0; i < ht->num_slots; i++) { //Loop through all of the indices
      set_iterate(ht-> set[i], arg, itemfunc); //Perform a given function on each 

    }
  }
}

/***************** hashtable_delete ******************/
void hashtable_delete(hashtable_t *ht, void( *itemdelete)(void *item)) 
{
  if (ht == NULL) {;
  } else {
    int i = 0;
    while (i < ht-> num_slots) {  // Use set_delete on each of the indices to delete them
      set_delete(ht-> set[i], itemdelete);
      i++;
    }
    count_free(ht-> set[i]); //free sets
    count_free(ht); //free hashtable
  }
}
