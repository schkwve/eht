#ifndef _EHT_PRIV_H
#define _EHT_PRIV_H

#include <easyht/eht.h>

#define EHT_PRIME_1 151
#define EHT_PRIME_2 163

void eht_resize_up(eht_hashtable *table);
void eht_resize_down(eht_hashtable *table);

eht_item *eht_new_item(const char *key, const char *value);
void eht_delete_item(eht_item *item);

eht_hashtable *eht_new_sized_table(const size_t base_size);

int eht_get_hash(const char *s, const int num_buckets, const int attempt);
int eht_hash(const char *s, const int a, const int m);

int eht_is_prime(const int x);
int eht_next_prime(int x);

#endif /* _EHT_PRIV_H */
