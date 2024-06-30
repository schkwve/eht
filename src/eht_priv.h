#ifndef _EHT_PRIV_H
#define _EHT_PRIV_H

#include <easyht/eht.h>

eht_item *eht_new_item(const char *key, const char *value);
void eht_delete_item(eht_item *item);

#endif /* _EHT_PRIV_H */
