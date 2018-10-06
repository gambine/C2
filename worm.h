#ifndef WORM_H
#define WORM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct book_t {
	size_t id;
	size_t author_id;
	size_t publisher_id;
	size_t* b_author_edges;
	size_t* b_citation_edges;
	size_t* b_publisher_edges;
	size_t n_author_edges;
	size_t n_citation_edges;
	size_t n_publisher_edges;
    bool is_visited;
} book_t;

typedef struct result_t {
	book_t** elements;
	size_t n_elements;
	size_t max;
} result_t;
//following code is from https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
typedef struct node_t {
	book_t* book;
	size_t level;
	struct node_t* parent;
	struct node_t* prev;
	struct node_t* next;
} node_t;
//following code is from https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
typedef struct queue_t {
	node_t * head;
	node_t * last;
} queue_t;


result_t* find_book(book_t* nodes, size_t count, size_t book_id);
result_t* find_books_by_author(book_t* nodes, size_t count, size_t author_id);
result_t* find_books_reprinted(book_t* nodes, size_t count, size_t publisher_id);
result_t* find_books_k_distance(book_t* nodes, size_t count, size_t book_id, uint16_t k);
result_t* find_shortest_distance(book_t* nodes, size_t count, size_t b1_id, size_t b2_id);
result_t* find_shortest_edge_type(book_t* nodes, size_t count, size_t a1_id, size_t a2_id);

#endif

