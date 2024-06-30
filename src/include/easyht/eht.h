#ifndef _EASYHT_EHT_H
#define _EASYHT_EHT_H

#include <stdint.h>

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
	size_t size;
	int count;
	eht_item **items;
} eht_hashtable;

/// FUNCTIONS ///

eht_hashtable *eht_new_hashtable();
void eht_delete_hashtable(eht_hashtable *table);

#endif /* _EASYHT_EHT_H */
