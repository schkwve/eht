#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <easyht/eht.h>

static eht_hashtable *table = NULL;

int test_setup_table()
{
	printf("Setting up a hashtable... ");

	table = eht_new_table();
	if (table == NULL) {
		printf("Failed (eht_new_table() returned NULL)\n");
		return -1;
	}

	eht_insert(table, "hello", "world!");
	eht_insert(table, "mine", "craft");
	eht_insert(table, "craft", "mine");
	eht_insert(table, "turtles", "snakes");

	printf("Done.\n");
	return 0;
}

int test_check_keyval(const char *key, const char *expected)
{
	printf("Checking value for key '%s'... ", key);
	char *value = eht_value(table, key);
	printf("\"%s\"\n", value);

	if (strcmp(value, expected) == 0) {
		return 0;
	}

	return -1;
}

int test_check_keyval_pairs()
{
	int status;
	bool error = false;

	status = test_check_keyval("hello", "world!");
	if (status != 0) {
		error = true;
	}

	status = test_check_keyval("mine", "craft");
	if (status != 0) {
		error = true;
	}

	status = test_check_keyval("craft", "mine");
	if (status != 0) {
		error = true;
	}

	status = test_check_keyval("turtles", "snakes");
	if (status != 0) {
		error = true;
	}

	return error ? -1 : 0;
}

int main(int argc, char **argv)
{
	int status = 0;
	bool error = false;

	if (test_setup_table() != 0) {
		error = true;
	}

	if (test_check_keyval_pairs() != 0) {
		error = true;
	}

	if (error) {
		printf("Failed!");
		status = -1;
	} else {
		printf("All tests passed!");
	}

	eht_delete_table(table);
	return status;
}