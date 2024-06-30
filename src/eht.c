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