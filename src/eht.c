#include <stdlib.h>
#include <string.h>

#include <easyht/eht.h>
#include <eht_priv.h>

/// PRIVATE FUNCTIONS ///

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

/// PUBLIC FUNCTIONS ///

eht_hashtable *eht_new_hashtable()
{
	eht_hashtable *new = malloc(sizeof(eht_hashtable));
	new->size = 32;
	new->count = 0;
	new->items = calloc((size_t)new->size, sizeof(eht_item *));

	return new;
}

void eht_delete_hashtable(eht_hashtable *table)
{
	for (int i = 0; i < table->size; i++) {
		eht_item *item = table->items[i];
		if (item != NULL) {
			eht_delete_item(item);
		}
	}
	free(table->items);
	free(table);
}