#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <easyht/eht.h>
#include <eht_priv.h>

/// NOTES ///

// TODO:
// - Implement xmalloc(), xcalloc()
//   |-> Safe functions with automatic error checking
//

/// PRIVATE VARIABLES ///
static eht_item EHT_DELETED_ITEM = {NULL,NULL};

/// PRIVATE FUNCTIONS ///

void eht_resize_up(eht_hashtable *table)
{
	const int new_size = table->base_size * 2;
	eht_resize_table(table, new_size);
}

void eht_resize_down(eht_hashtable *table)
{
	const int new_size = table->base_size / 2;
	eht_resize_table(table, new_size);
}

eht_item *eht_new_item(const char *key, const char *value)
{
	eht_item *new = malloc(sizeof(eht_item));
	new->key = strdup(key);
	new->value = strdup(value);

	return new;
}

void eht_delete_item(eht_item *item)
{
	free(item->key);
	free(item->value);
	free(item);
}

eht_hashtable *eht_new_sized_table(const size_t base_size)
{
	eht_hashtable *new = malloc(sizeof(eht_hashtable));
	new->base_size = base_size;
	new->size = eht_next_prime(new->base_size);
	new->count = 0;
	new->items = calloc(new->size, sizeof(eht_item *));

	return new;
}

int eht_get_hash(const char *s, const int num_buckets, const int attempt)
{
	const int hash_a = eht_hash(s, EHT_PRIME_1, num_buckets);
	const int hash_b = eht_hash(s, EHT_PRIME_2, num_buckets - 1);
	return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

int eht_hash(const char *s, const int a, const int m)
{
	const int len_s = strlen(s);
	long hash = 0;

	for (int i = 0; i < len_s; i++) {
		hash += (long)pow(a, len_s - (i+1)) * s[i];
		hash = hash % m;
	}

	return (int)hash;
}

int eht_is_prime(const int x)
{
	if (x < 2) {
		return -1;
	}
	if (x < 4) {
		return 1;
	}
	if ((x % 2) == 0) {
		return 0;
	}
	
	for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
		if ((x % i) == 0) {
			return 0;
		}
	}

	return 1;
}

int eht_next_prime(int x)
{
	while (eht_is_prime(x) != 1) {
		x++;
	}
	return x;
}

/// PUBLIC FUNCTIONS ///

/**
 * @brief Creates a new empty hashtable.
 * 
 * @return New hashtable
 */
eht_hashtable *eht_new_table()
{
	return eht_new_sized_table(EHT_INITIAL_BASE_SIZE);
}

/**
 * @brief Destroys a hashtable.
 * 
 * @param table
 * 		  Table to destroy
 */
void eht_delete_table(eht_hashtable *table)
{
	for (int i = 0; i < table->size; i++) {
		eht_item *item = table->items[i];
		if (item != NULL && item != &EHT_DELETED_ITEM) {
			eht_delete_item(item);
		}
	}
	free(table->items);
	free(table);
}

/**
 * @brief Resizes a hashtable.
 * 
 * @param table
 * 		  Hashtable to resize
 * 
 * @param base_size
 * 		  New base size
 */
void eht_resize_table(eht_hashtable *table, const int base_size)
{
	if (base_size < EHT_INITIAL_BASE_SIZE) {
		return;
	}

	eht_hashtable *new = eht_new_sized_table(base_size);
	for (int i = 0; i < table->size; i++) {
		eht_item *item = table->items[i];
		if (item != NULL && item != &EHT_DELETED_ITEM) {
			eht_insert(new, item->key, item->value);
		}
	}

	table->base_size = new->base_size;
	table->count = new->count;

	// discard new table
	const int tmp_size = table->size;
	table->size = new->size;
	new->size = tmp_size;

	eht_item **tmp_items = table->items;
	table->items = new->items;
	new->items = tmp_items;
	eht_delete_table(new);
}

/**
 * @brief Adds a key-value pair to the first available
 *        place in a hashtable. If the table is more than
 *        85% full, it is automatically resized.
 * 
 * @param table
 * 		  Hashtable
 * 
 * @param key
 * 		  Key to add
 * 
 * @param value
 * 		  Value to add
 */
void eht_insert(eht_hashtable *table, const char *key, const char *value)
{
	const int load = table->count * 100 / table->size;
	if (load > 85) {
		eht_resize_up(table);
	}

	eht_item *new = eht_new_item(key, value);
	int index = eht_get_hash(new->key, table->size, 0);
	eht_item *cur_item = table->items[index];

	for (int i = 1; cur_item != NULL; i++) {
		if (cur_item != &EHT_DELETED_ITEM) {
			if (strcmp(cur_item->key, key) == 0) {
				eht_delete_item(cur_item);
				table->items[index] = new;
				return;
			}
		}

		index = eht_get_hash(new->key, table->size, i);
		cur_item = table->items[index];
	}

	table->items[index] = new;
	table->count++;
}

/**
 * @brief Removes a key-value pair from a hashtable.
 *        If the table is more than 90% free, it is
 *        automatically resized.
 * 
 * @param table
 * 		  Hashtable
 * 
 * @param key
 * 		  Key to search for and remove
 */
void eht_remove(eht_hashtable *table, const char *key)
{
	const int load = table->count * 100 / table->size;
	if (load < 10) {
		eht_resize_down(table);
	}

	int index = eht_get_hash(key, table->size, 0);
	eht_item *item = table->items[index];

	for (int i = 1; item != NULL; i++) {
		if (item != &EHT_DELETED_ITEM) {
			if (strcmp(item->key, key) == 0) {
				eht_delete_item(item);
				table->items[index] = &EHT_DELETED_ITEM;
				table->count--;
				return;
			}
		}

		index = eht_get_hash(item->key, table->size, i);
		item = table->items[index];
	}
}

/**
 * @brief Gets the value of a key-value pair in a hashtable.
 * 
 * @param table
 * 		  Hashtable
 * 
 * @param key
 * 		  Key to search for
 * 
 * @return Value of key-value pair if the key was found;
 * 		   NULL otherwise.
 */
char *eht_value(eht_hashtable *table, const char *key)
{
	int index = eht_get_hash(key, table->size, 0);
	eht_item *item = table->items[index];

	for (int i = 1; item != NULL; i++) {
		if (item != &EHT_DELETED_ITEM) {
			if (strcmp(item->key, key) == 0) {
				return item->value;
			}
		}

		index = eht_get_hash(item->key, table->size, i);
		item = table->items[index];
	}

	return NULL;
}