#include "worm.h"
#include <string.h>

#define BUF_SIZE 4096

struct book_node_t* graph_loader(size_t* count, char* filename) {
  FILE* f = fopen(filename, "r");
  char buf[BUF_SIZE];
  fgets(buf, BUF_SIZE, f); //Gets number
  int total_number = strtol(buf, NULL, 10);
  struct book_node_t* graph = malloc(sizeof(struct book_node_t) * total_number);
  struct book_node_t node;
  char* ptr = NULL;
  for(int i=0; i < total_number; i++) {

    fgets(buf, BUF_SIZE, f); //Book ID
    node.id = strtol(buf, NULL, 10);
    //printf("%u\n", node.id);
    fgets(buf, BUF_SIZE, f); //Publisher ID
    node.publisher_id = strtol(buf, NULL, 10);
    fgets(buf, BUF_SIZE, f); //Author ID
    node.author_id = strtol(buf, NULL, 10);

    uint32_t cap = 16;
    uint32_t size = 0;
    node.related_publisher_books = malloc(sizeof(uint32_t) * cap);

    buf[BUF_SIZE-1] = 0;
    size_t length = BUF_SIZE-1;
    while(length == BUF_SIZE-1 && buf[length] != '\n') {
      fgets(buf, BUF_SIZE, f); //Publishers
      length = strlen(buf);
      for(ptr = strtok(buf, " "); ptr != NULL; ptr = strtok(NULL, " ")) {
        if(size == cap) { //
          cap = cap*2;
          node.related_publisher_books = realloc(node.related_publisher_books, sizeof(uint32_t) * cap);
        }
        if(strcmp("\n", ptr) != 0) {
          uint32_t k = strtol(ptr, NULL, 10);
          node.related_publisher_books[size] = k;
          size++;
        }
      }
    }
    node.n_publisher_size = size;


    cap = 10;
    size = 0;
    length = BUF_SIZE-1;
    while(length == BUF_SIZE-1 && buf[length] != '\n') {
      fgets(buf, BUF_SIZE, f);
      length = strlen(buf);
      node.same_author_books = malloc(sizeof(uint32_t) * cap);
      for(ptr = strtok(buf, " "); ptr != NULL; ptr = strtok(NULL, " ")) {
        if(size == cap) { //Authors
          cap = cap*2;
          node.same_author_books = realloc(node.same_author_books, sizeof(uint32_t) * cap);
        }
        if(strcmp("\n", ptr) != 0) {
          uint32_t k = strtol(ptr, NULL, 10);
          node.same_author_books[size] = k;
          size++;
        }
      }
    }
    node.n_author_size = size;


    cap = 10;
    size = 0;
    length = BUF_SIZE-1;
    while(length == BUF_SIZE-1 && buf[length] != '\n') {
      fgets(buf, BUF_SIZE, f);
      length = strlen(buf);
      node.cited_by = malloc(sizeof(uint32_t) * cap);
      for(ptr = strtok(buf, " "); ptr != NULL; ptr = strtok(NULL, " ")) {
        if(size == cap) { //Citations
          cap = cap*2;
          node.cited_by = realloc(node.cited_by, sizeof(uint32_t) * cap);
        }
        if(strcmp("\n", ptr) != 0) {
          uint32_t k = strtol(ptr, NULL, 10);
          node.cited_by[size] = k;
          size++;
        }
      }
    }
    node.n_cited_books = size;
    memcpy(&graph[i], &node, sizeof(node));
  }
  *count = total_number;

  return graph;
}


int main(int argc, char** argv) {

	size_t count = 0;
	struct book_node_t* nodes = graph_loader(&count, argv[1]);
	
	// Run and test queries here
	struct result_t * result = find_book(nodes, count, 17, 3);
	printf("find book id 17: %d\n", result->n_elements);
	for (int i = 0; i < result->n_elements; i++) {
		printf("book id is %d\n", result->elements[i].id);
	}
	
	return 0;
}
