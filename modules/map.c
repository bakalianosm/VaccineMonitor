


/* --------------------------------------------------------
 * ----------------------- MAP ------------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

#include <stdlib.h>

#include "map.h"
#include "linkedList.h"	
#define FIRST_HASH_SIZE 53


/* Load factor is for open addressing to keep the Map's Complexity properties */
#define MAX_LOAD_FACTOR 0.9

/* Map node struct */
struct map_node{
	Pointer key;		
	Pointer value;  	
} ;

/* Map struct */
struct map {
	LinkedList* array;			/* An array of lists */
	int capacity;				/* How many capacity we have set */
	int size;					/* How many elements we have added on the Map */
	CompareFunc compare;		/* Compare keys function */
	HashFunc hash_function;		
	DestroyFunc destroy_key;	
	DestroyFunc destroy_value;
};


Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	/* Allocate the memory that the Map needs */
	Map map = malloc(sizeof(*map));
	map->capacity = FIRST_HASH_SIZE;
	map->array = malloc(map->capacity * sizeof(LinkedList));

	/* Initialize empty lists */
	for (int i = 0; i < map->capacity; i++){
		map->array[i] = LL_create(free);
	}
	map->size = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}


int map_size(Map map) {
	return map->size;
}

/* Extend the hash table in case of the Load Factor increases */
static void rehash(Map map) {
	int old_capacity = map->capacity ;
	LinkedList* old_array = map->array;

	map->capacity *= 2 ;

	map->array = malloc(map->capacity * sizeof(LinkedList));

	for (int i = 0; i < map->capacity; i++){
		map->array[i] = LL_create(free);
	}

	map->size = 0;

	/* Re enter the elements on the resized map */
	for (int i = 0; i < old_capacity; i++){
		for(ListNode node = LL_first(old_array[i]) ;          
    	node != NULL;                          
    	node = LL_next(node)) { 
			MapNode m = LL_node_val(node);
			Pointer key = map_node_key(map,m);
			Pointer value = map_node_value(map,m);
			map_insert(map,key,value);
		}
	}

	for(int i = 0 ; i < old_capacity ; i++)
		LL_destroy(old_array[i]);

	free(old_array);
	return;
}


void map_insert(Map map, Pointer key, Pointer value) {
	
	/* Assume that the node to be inserted does not exists */
	bool already_in_map = false;

	/* Find where the key hashes */
	uint bucket;
	bucket = map->hash_function(key) % map->capacity;	
	
	/* Check if a node with the given key exists */
	MapNode toFind = map_find_node(map,key);
	if(toFind!=MAP_EOF) already_in_map = true;
	
	/* And if exists, replace the old node with the new */
	if(already_in_map){
		if (toFind->key != key && map->destroy_key != NULL)
			map->destroy_key(toFind->key);

		if (toFind->value != value && map->destroy_value != NULL)
			map->destroy_value(toFind->value);

		toFind->key = key;
		toFind->value = value;
	}
	else{
		/* If not exists */
		
		
		/* Create a new node */
		MapNode toInsert  = malloc(sizeof(*toInsert));
		toInsert->key = key;
		toInsert->value = value;

		/* And insert it on the corresponding bucket */
		LL_insert_after(map->array[bucket], NULL, toInsert);

		/* Increase size */
		map->size++;
	}

	/* If with the new insertion we overcome the load factor */
	float load_factor = (float)map->size / map->capacity;
	if (load_factor > MAX_LOAD_FACTOR) {
		// printf("LOAD FACTOR IS %f\n",load_factor);
		rehash(map); 
	}
}


bool map_remove(Map map, Pointer key) {
	
	/* Find where the key hashes */
	uint bucket;
	bucket = map->hash_function(key) % map->capacity;

	/* Make help nodes */
	ListNode prev=NULL;
	MapNode m;

	/* Iterate thourgh the bucket (--List--) */
	for(ListNode node = LL_first(map->array[bucket]) ;          
    node != NULL;                          
    node = LL_next(node)) {            
		m = LL_node_val(node);
		int* node_key = map_node_key(map,m );  
		
		
		if(map->compare(key,node_key)==0) {
			if (map->destroy_key != NULL)
				map->destroy_key(m->key);
			if (map->destroy_value != NULL)
				map->destroy_value(m->value);

			/* Remove node */	
			LL_remove_next_item(map->array[bucket],prev);

			/* Decrease size */
			map->size--;
			return true;
		} 
		prev = node;
	}
	
	/* If the node did not found */
	return false;
}



Pointer map_find(Map map, Pointer key) {
	MapNode node = map_find_node(map, key);
	if (node != MAP_EOF)
		return node->value;
	else
		return NULL;
}


DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = map->destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = map->destroy_value;
	map->destroy_value = destroy_value;
	return old;
}


void map_destroy(Map map) {

	/* Iterate thought the list array */
	for (int i = 0 ; i< map->capacity ; i++){
		/* And for each list node */
		for(ListNode node = LL_first(map->array[i]) ;          
    	node != NULL;                          
    	node = LL_next(node)) {   

			/* Get the Map node */
			MapNode m = LL_node_val(node);

				/* And destroy it's content */
				if (map->destroy_key != NULL)
					map->destroy_key(m->key);
				if (map->destroy_value != NULL)
					map->destroy_value(m->value);
			}
		
		/* Destroy the list */
		LL_destroy(map->array[i]);
	}
	
	/* Free the List Array */
	free(map->array);

	/* And the whole map */
	free(map);
}

/* Map iteration */

MapNode map_first(Map map) {

	/* Return the first element of the first list */
	if(map->array[0]!=NULL)
		if(LL_first(map->array[0])!= NULL) return LL_node_val(LL_first(map->array[0]));
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {

	/* Find where the key hases */
	uint bucket;
	bucket = map->hash_function(node->key) % map->capacity;
	

	/* And then find the List Node that contains the given node */
	ListNode lnode ;
	for( lnode = LL_first(map->array[bucket]) ;          
    lnode != NULL;                          
    lnode = LL_next(lnode)) {            
		MapNode m = (MapNode)LL_node_val(lnode); /* Get the node value */
		Pointer node_key = map_node_key(map,m );  		  /* Get the key */
		Pointer node_value = map_node_value(map,m );	  /* Get the value */
		/* If the node found , stop */
		if(map->compare(node->key,node_key)==0 && map->compare(node->value,node_value)==0) break; 
	}

	/* If exists next ,return it */
	if(LL_next(lnode)!=NULL) 
		return LL_node_val(LL_next(lnode));
	else{
	/* Else search on the next bucket */
		bucket++;
		if(bucket <= map->capacity ) {
			if(LL_first(map->array[bucket])!=NULL)
				return LL_node_val(LL_first(map->array[bucket]));
		}
	}
	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode node) {
	return node->key;
}

Pointer map_node_value(Map map, MapNode node) {
	return node->value;
}

MapNode map_find_node(Map map, Pointer key) {
	
	uint bucket;
	bucket = map->hash_function(key) % map->capacity;
	
	/* Iterate throught the bucket (--List--) */
	for(ListNode node = LL_first(map->array[bucket]) ;          
    node != NULL;                         
    node = LL_next(node)) {            
		MapNode m = LL_node_val(node);
		Pointer node_key = map_node_key(map,m );  
		if(map->compare(key,node_key)==0) return m;
		
	}
	return MAP_EOF;
}


void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

uint hash_string(Pointer value) {
	
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				
}