#include <stdio.h>
#include <easyht/eht.h>

int main(int argc, char **argv)
{
	eht_hashtable *table = eht_new_hashtable();
	eht_delete_hashtable(table);
	return 0;
}