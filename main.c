#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "worm.h"

#define MAX_BUFFER 65535

// Reads given graph file and returns a book graph.
book_t* graph_loader(size_t* count, char* filename) {

	char buffer[MAX_BUFFER];
	size_t n_books = 0;

	// Open graph file
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		perror("Fatal error! Unable to open graph file");
		return NULL;
	}

	// Read book count
	if (fgets(buffer, MAX_BUFFER, f) == NULL || sscanf(buffer, "%zu", &n_books) == 0) {
		fprintf(stderr, "Fatal error! Unable to read book count.\n");
		return NULL;
	}

	book_t* graph = malloc(sizeof(book_t) * n_books);

	// Read books
	for (size_t i = 0; i < n_books; i++) {

		// Read book ID
		if (fgets(buffer, MAX_BUFFER, f) == NULL || sscanf(buffer, "%zu", &graph[i].id) == 0) {
			fprintf(stderr, "Fatal error! Unable to read book ID for book %zu.\n", i);
			return NULL;
		}

		// Read publisher ID
		if (fgets(buffer, MAX_BUFFER, f) == NULL || sscanf(buffer, "%zu", &graph[i].publisher_id) == 0) {
			fprintf(stderr, "Fatal error! Unable to read publisher ID for book %zu.\n", i);
			return NULL;
		}

		// Read author ID
		if (fgets(buffer, MAX_BUFFER, f) == NULL || sscanf(buffer, "%zu", &graph[i].author_id) == 0) {
			fprintf(stderr, "Fatal error! Unable to read author ID for book %zu.\n", i);
			return NULL;
		}

		size_t cap = 10;
		size_t size = 0;

		buffer[MAX_BUFFER - 1] = '\0';
		size_t length = MAX_BUFFER - 1;
		graph[i].b_publisher_edges = malloc(sizeof(size_t) * cap);

		// Read publisher edges
		while (length == MAX_BUFFER - 1 && buffer[length] != '\n') {
			if (fgets(buffer, MAX_BUFFER, f) == NULL) {
				fprintf(stderr, "Fatal error! Unable to read publishers for book %zu.\n", i);
			}

			length = strlen(buffer);
			for (char* s = strtok(buffer, " "); s != NULL; s = strtok(NULL, " ")) {
				if (size == cap) {
					cap = cap * 2;
					graph[i].b_publisher_edges = realloc(graph[i].b_publisher_edges, sizeof(size_t) * cap);
				}
				if (strcmp("\n", s) != 0) {
					size_t k = strtol(s, NULL, 10);
					graph[i].b_publisher_edges[size] = k;
					size++;
				}
			}
		}
		graph[i].n_publisher_edges = size;

		cap = 10;
		size = 0;

		buffer[MAX_BUFFER - 1] = '\0';
		length = MAX_BUFFER - 1;
		graph[i].b_author_edges = malloc(sizeof(size_t) * cap);

		// Read author edges
		while (length == MAX_BUFFER - 1 && buffer[length] != '\n') {
			if (fgets(buffer, MAX_BUFFER, f) == NULL) {
				fprintf(stderr, "Fatal error! Unable to read publishers for book %zu.\n", i);
			}

			length = strlen(buffer);
			for (char* s = strtok(buffer, " "); s != NULL; s = strtok(NULL, " ")) {
				if (size == cap) {
					cap = cap * 2;
					graph[i].b_author_edges = realloc(graph[i].b_author_edges, sizeof(size_t) * cap);
				}
				if (strcmp("\n", s) != 0) {
					size_t k = strtol(s, NULL, 10);
					graph[i].b_author_edges[size] = k;
					size++;
				}
			}
		}
		graph[i].n_author_edges = size;

		cap = 10;
		size = 0;

		buffer[MAX_BUFFER - 1] = '\0';
		length = MAX_BUFFER - 1;
		graph[i].b_citation_edges = malloc(sizeof(size_t) * cap);

		// Read citation edges
		while (length == MAX_BUFFER - 1 && buffer[length] != '\n') {
			if (fgets(buffer, MAX_BUFFER, f) == NULL) {
				fprintf(stderr, "Fatal error! Unable to read citations for book %zu.\n", i);
			}

			length = strlen(buffer);
			for (char* s = strtok(buffer, " "); s != NULL; s = strtok(NULL, " ")) {
				if (size == cap) {
					cap *= 2;
					graph[i].b_citation_edges = realloc(graph[i].b_citation_edges, sizeof(size_t) * cap);
				}
				if (strcmp("\n", s) != 0) {
					size_t k = strtol(s, NULL, 10);
					graph[i].b_citation_edges[size] = k;
					size++;
				}
			}
		}
		graph[i].n_citation_edges = size;
	}

	*count = n_books;
	return graph;
}



int main(int argc, char** argv) {

	// Example usage

	size_t count = 0;
	book_t* graph = graph_loader(&count, "sample.graph");
	if (graph == NULL) {
		return 1;
	}

	

	for (size_t i = 0; i < count; i++) {
		free(graph[i].b_author_edges);
		free(graph[i].b_publisher_edges);
        free(graph[i].b_citation_edges);
	}

	free(graph);

	return 0;
}

