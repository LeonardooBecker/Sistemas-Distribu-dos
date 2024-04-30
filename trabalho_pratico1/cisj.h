#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POW_2(num) (1<<(num))
#define VALID_J(j, s) ((POW_2(s-1)) >= j)

/* |-- node_set.h */
typedef struct node_set {
	int* nodes;
	size_t size;
	size_t offset;
} node_set;


node_set* cis(int i, int s);

static node_set* set_new(size_t size);

static void set_insert(node_set* nodes, int node);

static void set_merge(node_set* dest, node_set* source);

static void set_free(node_set* nodes);

node_set* cis(int i, int s);