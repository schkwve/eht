#ifndef _EHT_PRIV_H
#define _EHT_PRIV_H

#include <easyht/eht.h>

#define EHT_PRIME_1 151
#define EHT_PRIME_2 163

eht_item *eht_new_item(const char *key, const char *value);
void eht_delete_item(eht_item *item);

int eht_get_hash(const char *s, const int num_buckets, const int attempt);
int eht_hash(const char *s, const int a, const int m);

#endif /* _EHT_PRIV_H */
