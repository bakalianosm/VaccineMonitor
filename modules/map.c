


/* --------------------------------------------------------
 * ----------------------- MAP ------------------------
 * ------------------ IMPLEMENTATION ----------------------
 * --------------------------------------------------------
 */

#include <stdlib.h>

#include "map.h"
#include "linkedList.h"	
#define FIRST_HASH_SIZE 53
// Χρησιμοποιούμε open addressing, οπότε σύμφωνα με την θεωρία, πρέπει πάντα να διατηρούμε
// τον load factor του  hash table μικρότερο ή ίσο του 0.5, για να έχουμε αποδoτικές πράξεις
#define MAX_LOAD_FACTOR 0.9

// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map)
struct map_node{
	Pointer key;		// Το κλειδί που χρησιμοποιείται για να hash-αρουμε
	Pointer value;  	// Η τιμή που αντισοιχίζεται στο παραπάνω κλειδί
} ;

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
	LinkedList* array;				// Ο πίνακας σπό λίστες που θα χρησιμοποιήσουμε για το map (remember, φτιάχνουμε ένα hash table)
	int capacity;				// Πόσο χώρο έχουμε δεσμεύσει.
	int size;					// Πόσα στοιχεία έχουμε προσθέσει
	CompareFunc compare;		// Συνάρτηση για σύγκρηση δεικτών, που πρέπει να δίνεται απο τον χρήστη
	HashFunc hash_function;		// Συνάρτηση για να παίρνουμε το hash code του κάθε αντικειμένου.
	DestroyFunc destroy_key;	// Συναρτήσεις που καλούνται όταν διαγράφουμε έναν κόμβο απο το map.
	DestroyFunc destroy_value;
};


Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	// Δεσμεύουμε κατάλληλα τον χώρο που χρειαζόμαστε για το hash table
	Map map = malloc(sizeof(*map));
	map->capacity = FIRST_HASH_SIZE;
	map->array = malloc(map->capacity * sizeof(LinkedList));

	// Αρχικοποιούμε τις λίστες που έχουμε σαν διαθέσιμες.
	for (int i = 0; i < map->capacity; i++){
		map->array[i] = LL_create(free);
	}
	map->size = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;
}

// Συνάρτηση για την επέκταση του Hash Table σε περίπτωση που ο load factor μεγαλώσει πολύ.
static void rehash(Map map) {
	int old_capacity = map->capacity ;
	LinkedList* old_array = map->array;

	map->capacity *= 2 ;

	map->array = malloc(map->capacity * sizeof(LinkedList));

	for (int i = 0; i < map->capacity; i++){
		map->array[i] = LL_create(free);
	}

	map->size = 0;

	for (int i = 0; i < old_capacity; i++){
		for(ListNode node = LL_list_first(old_array[i]) ;          // ξενικάμε από τον πρώτο κόμβο
    	node != NULL;                          // μέχρι να φτάσουμε στο EOF
    	node = LL_list_next(old_array[i], node)) { 
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

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.

void map_insert(Map map, Pointer key, Pointer value) {
	// Σκανάρουμε το Hash Table μέχρι να βρούμε διαθέσιμη θέση για να τοποθετήσουμε το ζευγάρι,
	// ή μέχρι να βρούμε το κλειδί ώστε να το αντικαταστήσουμε.
	
	bool already_in_map = false;

	// Βρίσκουμε που χασάρει το κλειδί
	uint bucket;
	bucket = map->hash_function(key) % map->capacity;	
	
	// Ο κόμβος που θα ελέγχξουμε αν υπάρχει ήδη (το κλειδί συγκεκριμένα)
	MapNode toFind = map_find_node(map,key);
	if(toFind!=MAP_EOF) already_in_map = true;
	
	// Σε αυτή την περίπτωση , κοιτάω το γεγονός το κλειδί αυτό να υπάρχει
	// ήδη στο  Map. Αν υπάρχει, ουσιαστικά του αλλάζω του value 
	if(already_in_map){
		if (toFind->key != key && map->destroy_key != NULL)
			map->destroy_key(toFind->key);

		if (toFind->value != value && map->destroy_value != NULL)
			map->destroy_value(toFind->value);

		toFind->key = key;
		toFind->value = value;
	}
	// Αν δεν υπάρχει στο Map το key 
	else{

		// Δημιουργία του κόμβου προς εισαγωγή
		MapNode toInsert  = malloc(sizeof(*toInsert));
		toInsert->key = key;
		toInsert->value = value;

		// Εισαγωή νέου κόμβου στο Map
		LL_insert_at_start(map->array[bucket], toInsert);

		// Αύξηση του μεγέθους του
		map->size++;
	}

	// Αν με την νέα εισαγωγή ξεπερνάμε το μέγιστο load factor, πρέπει να κάνουμε rehash
	float load_factor = (float)map->size / map->capacity;
	if (load_factor > MAX_LOAD_FACTOR) {
		// printf("LOAD FACTOR IS %f\n",load_factor);
		rehash(map); 
	}
}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, Pointer key) {
	
	// Βρίσκουμε που χασάρει ο κόμβος του ορίσματος 
	uint bucket;
	bucket = map->hash_function(key) % map->capacity;

	// Φτιάχνουμε βοηθητικούς κόμβους
	ListNode prev=NULL;
	MapNode m;

	// Διατρέχουμε τη λίστα
	for(ListNode node = LL_list_first(map->array[bucket]) ;          
    node != NULL;                          
    node = LL_list_next(map->array[bucket], node)) {            
		m = LL_node_val(node);
		int* node_key = map_node_key(map,m );  
		
		// Μόλις βρεθεί στη λίστα ο κόμβος που θα διαγραφεί
		// Σε αυτό το σημείο αν είναι ο πρώτος θα διαγραφεί με τη χρήση
		// της LL_remove_next_item αλλιώς θα προχωρήσει και θα ενημερώσει
		// τον κόμβο prev και θα διαγράψουμε τον επόμενο του prev.
		if(map->compare(key,node_key)==0) {
			if (map->destroy_key != NULL)
				map->destroy_key(m->key);
			if (map->destroy_value != NULL)
				map->destroy_value(m->value);

			// Αφαιρούμε τον κόμβο	
			LL_remove_next_item(map->array[bucket],prev);

			// Μειώνουμε το μέγεθος
			map->size--;
			return true;
		} 
		prev = node;
	}
	// Αν δε δούλεψε σωστά ο αλγόριθμος ή εάν δε βρέθηκε 
	// ο κόμβος επιστρέφει false
	return false;
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

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

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {

	// Διατρέχουμε όλες τις λίστες του πίνακα λιστών 
	for (int i = 0 ; i< map->capacity ; i++){
		// Και για κάθε κόμβο λίστας 
		for(ListNode node = LL_list_first(map->array[i]) ;          
    	node != NULL;                          
    	node = LL_list_next(map->array[i], node)) {   

			// Φέρνουμε στη μνήμη τον αντίστοιχο MapNode που περιέχει        
			MapNode m = LL_node_val(node);

				// Και καταστρέφουμε το περιεχόμενό του
				if (map->destroy_key != NULL)
					map->destroy_key(m->key);
				if (map->destroy_value != NULL)
					map->destroy_value(m->value);
			}
		// Εφόσον αυτό έχει γίνει για κάθε περιεχόμενο κόμβου λίστας 
		// Ελευθερώνουμε και τη λίστα
		LL_destroy(map->array[i]);
	}
	
	// Ελευθερώνουμε τον πίνακα λιστών 
	free(map->array);

	// Καθώς και το όλο Map
	free(map);
}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

MapNode map_first(Map map) {
	if(map->array[0]!=NULL)
		if(LL_list_first(map->array[0])!= NULL) return LL_node_val(LL_list_first(map->array[0]));
	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {

	// Βρίσκουμε που χασάρει ο κόμβος του ορίσματος 
	// ώστε να επιστρέψουμε τον επόμενο κόμβο στην λίστα 
	uint bucket;
	bucket = map->hash_function(node->key) % map->capacity;
	
	// Βρίσκουμε τον κόμβο λίστας που περιλαμβάνει τον node
	// Διατρέχουμε τη λίστα απ την αρχή μέχρι να βρούμε τον κόμβο
	// που περιέχει τον node του ορίσματος

	ListNode lnode ;
	for( lnode = LL_list_first(map->array[bucket]) ;          
    lnode != NULL;                          
    lnode = LL_list_next(map->array[bucket], lnode)) {            
		MapNode m = (MapNode)LL_node_val(lnode); // παίρνουμε το περιεχόμενο του κόμβου λίστας
		Pointer node_key = map_node_key(map,m );  		  // το κλειδί του κόμβου
		Pointer node_value = map_node_value(map,m );	  //η τιμή του κόμβου
		// Αν ο κόμβος είναι αυτός που μας ενδιαφέρει , τον βρήκαμε και σταματάμε
		if(map->compare(node->key,node_key)==0 && map->compare(node->value,node_value)==0) break; 
	}

	// Αν υπάρχει επόμενος του στη λίστα , τον επιστρέφω 
	if(LL_list_next(map->array[bucket],lnode)!=NULL) 
		return LL_node_val(LL_list_next(map->array[bucket],lnode));
	else{
	// Αλλιώς πάω στο επόμενο bucket και επιστρέφω τον πρώτο κόμβο .	
		bucket++;
		if(bucket <= map->capacity ) {
			if(LL_list_first(map->array[bucket])!=NULL)
				return LL_node_val(LL_list_first(map->array[bucket]));
		}
	}
	// Διαφορετικά κάτι πήγε λάθος
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
	
	// διατρέχω τη λίστα 
	for(ListNode node = LL_list_first(map->array[bucket]) ;          // ξενικάμε από τον πρώτο κόμβο
    node != NULL;                          // μέχρι να φτάσουμε στο EOF
    node = LL_list_next(map->array[bucket], node)) {            // μετάβαση στον επόμενο κόμβο
		MapNode m = LL_node_val(node);
		Pointer node_key = map_node_key(map,m );  // η τιμή του συγκεκριμένου κόμβου
		if(map->compare(key,node_key)==0) return m;
		
	}
	return MAP_EOF;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

uint hash_string(Pointer value) {
	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}