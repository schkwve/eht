#ifndef _EASYHT_EHT_H
#define _EASYHT_EHT_H

#include <stdint.h>

/// USER-CHANGEABLE VARIABLES ///

#ifndef EHT_INITIAL_BASE_SIZE
#define EHT_INITIAL_BASE_SIZE 14
#endif

/// DATA STRUCTURES ///

/**
 * @brief Key-value pair structure.
 */
typedef struct _eht_item {
	char *key;
	char *value;
} eht_item;

/**
 * @brief Hash table structure.
 */
typedef struct _eht_hashtable {
	size_t base_size;
	size_t size;
	int count;
	eht_item **items;
} eht_hashtable;

/// FUNCTIONS ///

eht_hashtable *eht_new_table();
void eht_delete_table(eht_hashtable *table);
void eht_resize_table(eht_hashtable *table, const int base_size);

void eht_insert(eht_hashtable *table, const char *key, const char *value);
void eht_remove(eht_hashtable *table, const char *key);
char *eht_value(eht_hashtable *table, const char *key);

#endif /* _EASYHT_EHT_H */
