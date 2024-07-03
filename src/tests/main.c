#include <stdio.h>
#include <easyht/eht.h>

int main(int argc, char **argv)
{
	eht_hashtable *table = eht_new_table();

	eht_delete_table(table);
	return 0;
}