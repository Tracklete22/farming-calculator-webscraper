 /*
  *set.c - see 'set.h' for more information
  *
  * Myles Holt, 18 July 2018
  */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdbool.h>
 #include "set.h"
 #include "memory.h"

 /**************** file-local global variables ****************/
 /* none */

 /************** global types ***************/
 typedef struct set {
   struct setnode *head; 
 }
 set_t;

 /************** local types ***************/
 typedef struct setnode {
   char *key;
   void *item;
   struct setnode *next;
 }
 setnode_t;

 /************** local functions ***************/
 static setnode_t *new_node(const char *key, void *item);


 /************** set_new function ***************/
 set_t *set_new(void) {

   set_t *set = count_malloc(sizeof(set_t)); //create a new set

   if (set == NULL) { //If failed to allocate memory, return null
     return NULL;
   } else {
     set->head = NULL; //set head (front of the linked list) to initially be null
     return set;
   }
 }

 /************** new_node function ***************/
 static setnode_t *new_node(const char *key, void *item) 
 {

   setnode_t *node = count_malloc(sizeof(setnode_t)); // allocate memory for a new node

   if (node == NULL) {
     count_free(node);
     return NULL;
   } else {
     char *keydupe = count_malloc(strlen(key) + 1); //create memory for the key, which will be copied in the following "strcopy function"
     if (keydupe == NULL) {
       return NULL;
     } else {
       strcpy(keydupe, key); //copy over a duplicate of the key
       node->key = keydupe; // Initialize each variable within the node
       node->item = item;
       node->next = NULL;
     }
     return node;
   }
 }

 /************** set_insert function ***************/
 bool set_insert(set_t *set,
   const char *key, void *item) 
 {

   if (set == NULL || key == NULL || item == NULL) {
     return false;
   }

   if (set_find(set, key) != NULL) { //Return false if the given key is found within the set
     return false;

   } else {

     setnode_t *new = new_node(key, item); // If they key is not found in the set, create a new node

     if (new == NULL) {
       return false;
     }

     new->next = set->head; // Insert the new node at the head, and set the head equal to the new node in order to make the new node the head of the list
     set->head = new;

   }
   return true;
 }

 /************** set_find function ***************/
 void *set_find(set_t *set,
   const char *key) 
 {

   if (set == NULL || key == NULL) {
     return NULL;
   }

   for (setnode_t *node = set-> head; node != NULL; node = node->next) { // Iterate through all of the nodes in the set
     if (strcmp(node->key, key) == 0) { // If the key (chosen by the user) is equal to any of the keys in the set, return that node
       return node->item;
     }
   }

   return NULL;
 }

 /************** set_print function ***************/
 void set_print(set_t *set, FILE *fp, void( *itemprint)(FILE *fp,
   const char *key, void *item)) 
 {
   if (set == NULL) {
     fputs("(null)", fp); //if set is null, print (null)
   }

   if ( *itemprint == NULL) {
     fputs("{}", fp);
   }

   if (fp != NULL) {
     if (set != NULL) {
       fputc('{', fp); // Start the beginnig of the set using this character

       for (setnode_t *node = set->head; node != NULL; node = node->next) {
         if (itemprint != NULL) {
           (*itemprint)(fp, node->key, node->item); // Call the itemprint function to print the key and item of the node (this will be given in testing file)
           fputc(", ", fp);
         }
       }
       fputc('}', fp); // Finally, close the set
     }
   }
 }

 /************** set_iterate function ***************/
 void set_iterate(set_t *set, void *arg, void( *itemfunc)(void *arg,
   const char *key, void *item)) 
 {
   if (set == NULL || itemfunc == NULL) {;
   } else {

     for (setnode_t *node = set->head; node != NULL; node = node->next) {
       ( *itemfunc)(arg, node->key, node->item); // Loop through the set and call a given function on each of the nodes
     }
   }
 }

 /************** set_delete function ***************/
 void set_delete(set_t *set, void( *itemdelete)(void *item)) 
 {
   if (set != NULL) {
     for (setnode_t *node = set->head; node != NULL;) { // Loop through the set and call itemdelete on each of the nodes
       if (itemdelete != NULL) {
         ( *itemdelete)(node->item);
       }
       setnode_t *next = node->next; //Create a temporary variable to store the next node, otherwise the pointer to the next node will be lost
       count_free(node->key); // free both the key and the node itself once deleted
       count_free(node);
       node = next;
     }

     count_free(set); // Free the entire set
   }
 }
