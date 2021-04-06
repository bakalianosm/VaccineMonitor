


/* -------------------------------------------------------
 * ----------------------- MAP ---------------------------
 * ------------------ HEADER FILE ------------------------
 * -------------------------------------------------------
 */

#pragma once 

#include "common.h"



typedef struct map* Map;


/* Creates and returns a map */
Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value);

/* Returns how many items are in the map */
int map_size(Map map);

/* Inserts on map, a node with "key" and "value" . If the current key exists, we replace the old value with the new */
void map_insert(Map map, Pointer key, Pointer value);

/* Removes the node with the given key */
bool map_remove(Map map, Pointer key);

/* Returns the value of the node with given key */
Pointer map_find(Map map, Pointer key);

/* Setters for destroy_key and destroy_value functions */
DestroyFunc map_set_destroy_key  (Map map, DestroyFunc destroy_key  );
DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value);

/* free the memory that the map has allocated */
void map_destroy(Map map);



/* Map iteration functions */
#define MAP_EOF (MapNode)0

typedef struct map_node* MapNode;

/* Returns the first element of the Map or NULL if the Map is empty */
MapNode map_first(Map map);

/* Returns the next of given node, or NULL if given node has not next node */
MapNode map_next(Map map, MapNode node);

/* Returns the key of the given node */
Pointer map_node_key(Map map, MapNode node);

/* Returns the value of the given node */
Pointer map_node_value(Map map, MapNode node);


/* Finds and returns the node that has key or NULL if node not exists */
MapNode map_find_node(Map map, Pointer key);


/* Some useful hash functions */

/* Hash function definition */
typedef uint (*HashFunc)(Pointer);

uint hash_string(Pointer value);		
uint hash_int(Pointer value);			
uint hash_pointer(Pointer value);		
uint hash_vaccine(Pointer value);

/* Set the hash function for the given Map */
void map_set_hash_function(Map map, HashFunc hash_func);
