#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#include "worm.h"

size_t g_nthreads = 4;

// Returns result set containing book with given id.
result_t* find_book(book_t* nodes, size_t count, size_t book_id) {
	result_t* result = malloc(sizeof(result_t));
	result -> max = 10;
	result -> elements = NULL;
	result -> n_elements = 0;
	
	for(size_t i = 0; i < count; i++){
		//find the book_id
		if(nodes[i].id == book_id){
			result -> n_elements++;
			if(result -> elements == NULL){
				result -> elements = malloc(sizeof(book_t*) * result->max);
			}
			
			if(result -> max == result -> n_elements){
				result -> max = result -> max << 1;
				result -> elements = realloc(result -> elements, sizeof(book_t*) * result->max);
			}
			result -> elements[result->n_elements-1] = &nodes[i];
            break;
		}
	}
	return result;
}

// Returns result set containing books by given author.
result_t* find_books_by_author(book_t* nodes, size_t count, size_t author_id) {
	result_t *result = malloc(sizeof(result_t));
	result -> elements = NULL;
	result -> n_elements = 0;
	result -> max = 10;
	
	
	for(size_t i=0;i<count;i++){
		//find the author_id
		if(nodes[i].author_id == author_id){
			result -> n_elements++;
			if(result -> elements == NULL){
				result -> elements = malloc(sizeof(book_t*) * result -> max);				
			}
			if(result -> max == result -> n_elements){
				result -> max = result -> max << 1;
				result -> elements = realloc(result -> elements,sizeof(book_t*) * result -> max); 
			}
			result -> elements[result->n_elements-1] = &nodes[i];
		}
	}
	
	return result;
}

result_t* find_books_publisher(book_t* nodes, size_t count, size_t publisher_id) {
	result_t *result = malloc(sizeof(result_t));
	result -> elements = NULL;
	result -> n_elements = 0;
	result -> max = 10;
	
	for(size_t i=0;i<count;i++){
		//find the publisher_id
		if(nodes[i].publisher_id == publisher_id){
			result -> n_elements++;
			if(result -> elements == NULL){
				result -> elements = malloc(sizeof(book_t*) * result -> max);				
			}
			if(result -> max == result -> n_elements){
				result -> max = result -> max << 1;
				result -> elements = realloc(result -> elements,sizeof(book_t*) * result -> max); 
			}
			result -> elements[result->n_elements-1] = &nodes[i];
		}
	}
	
	return result;
}

// Returns result set containing books that have been reprsize_ted by a different publisher.
//first create a function that can find the book with same publisher_id
//second compare each of those books to the rest of other books and see which has same id, author_id, but diffenret publisher_id
//store the matched set into the result and return it.
result_t* find_books_reprinted(book_t* nodes, size_t count, size_t publisher_id) {
	result_t *books = find_books_publisher(nodes, count, publisher_id);
	book_t *book1,*book2;
	result_t *result = malloc(sizeof(result_t));
	result -> elements = NULL;
	result -> n_elements = 0;
	result -> max = 10;
	
	
	for(size_t i=0;i<books->n_elements;i++)
	{
		book1 = books -> elements[i];
		
		for(size_t j = 0; j < count; j++)
		{
			book2 = &nodes[j];
			if(book1 -> id == book2 -> id
			   && book1 -> author_id == book2 -> author_id
			   && book1 -> publisher_id != book2 -> publisher_id){
				
				result -> n_elements++;
				if(result -> elements == NULL){
					result -> elements = malloc(sizeof(book_t*) * result -> max);				
				}
				if(result -> max == result -> n_elements){
					result -> max = result -> max << 1;
					result -> elements = realloc(result -> elements,sizeof(book_t*) * result -> max); 
				}
				result -> elements[result -> n_elements-1] = book2;
			}
		}	
	}
    if (books->elements != NULL) {
        free(books->elements);
    }
	free(books);
	return result;
}

//following code is modified from https://www.tutorialspoint.com/data_structures_algorithms/doubly_linked_list_program_in_c.htm
//is list empty
bool isEmpty(queue_t* queue) {
   return queue->head == NULL;
}


void enqueue(queue_t* queue, node_t* link) {
   if(isEmpty(queue)) {
      //make it the last link
     queue->last = link;
	 queue->head = link;
   } else {
      //make link a new last link
      queue->last->next = link;

      //mark old last node as prev of new link
      link->prev = queue->last;
   }

   //point last to new last node
   queue->last = link;

}

node_t* dequeue(queue_t* queue) {
	//save reference to first link
   node_t* tempLink = queue->head;

   //if only one link
   if(queue->head->next == NULL){
      queue->last = NULL;
   } else {
      queue->head->next->prev = NULL;
   }

   queue->head = queue->head->next;
   //return the deleted link
   return tempLink;
}


// Returns result set containing books that are k distance from given book.(include the first and k)
result_t* find_books_k_distance(book_t* nodes, size_t count, size_t book_id, uint16_t k) {
    //find all the books according to the book id 
	result_t *result = find_book(nodes, count, book_id);
	if (result->elements == NULL) {
		return result;
	}
    
    //returen the visited array(result)	store all the node that was visited by bfs in here
    result_t* visited = malloc(sizeof(result_t));
	visited->elements = NULL;
	visited->n_elements = 0;
    visited -> max = 10;
    
    

    //get the one book 
	//free the memory
	book_t* thebook = result->elements[0];
	free(result->elements);
	free(result);

    // for all vertices u in G
    //     u.seen = false
    for(size_t i = 0; i < count; i++)
    {
        nodes[i].is_visited = false;
    }

    // put the book we find into the node
	node_t* firstNode = malloc(sizeof(node_t));
	firstNode->book = thebook;
	firstNode->level = 0;
	firstNode->parent = NULL;
	firstNode->prev = NULL;
	firstNode->next = NULL;

    //initialize the queue 
    //use it for bfs
	queue_t* queue = malloc(sizeof(queue_t));
	queue->head = firstNode;
	queue->last = firstNode;

	// this one is to store all malloced nodes, so they can be free in the end
	//if we don't have this, cannot free the queue while frist time dequeue, it is null
	size_t max = 10;
	node_t ** links = malloc(sizeof(node_t*) * max);
	links[0] = firstNode;
	size_t n_links = 1;

    //  while(!q.empty())
    //     u = q.deque()
	do {
		node_t* currentNode = dequeue(queue);

		if (currentNode->level > k) {
			break;
		}

        // if u.seen is false
        //     u.seen = true
		//store it in the array
		if (currentNode->book->is_visited == false) {
            currentNode->book->is_visited = true;

			visited -> n_elements++;
			if(visited -> elements == NULL){
				visited -> elements = malloc(sizeof(book_t*) * visited -> max);				
			}
			if(visited -> max == visited -> n_elements){
				//visited -> max *= 2
				visited -> max = visited -> max << 1;
				visited -> elements = realloc(visited -> elements,sizeof(book_t*) * visited -> max); 
			}
			visited -> elements[visited->n_elements-1] = currentNode->book;

			

			// for all neighbours w of u
			for(size_t i = 0; i < currentNode->book->n_citation_edges; i++) {
				size_t neighbour_id = currentNode->book->b_citation_edges[i];

				// result_t *n_result = find_book(nodes, count, neighbour_id);
				book_t* neighbour_book = nodes + neighbour_id;
				node_t* neighbour_node = malloc(sizeof(node_t));
				neighbour_node->book = neighbour_book;
				neighbour_node->level = currentNode->level + 1;
				
				enqueue(queue, neighbour_node);

				if (n_links == max) {
					max = max << 1;
					links = realloc(links, sizeof(node_t *) * max);
				}
				links[n_links] = neighbour_node;
				n_links++;
			}
		}
	}while(!isEmpty(queue));

	for (size_t i = 0; i < n_links; i++) {
		free(links[i]);
	}
	free(links);
	free(queue);

	return visited;


	
}

// Returns result set containing books in shortest path between book 1 and 2.
result_t* find_shortest_distance(book_t* nodes, size_t count, size_t b1_id, size_t b2_id) {
   //find the book by book1 id	
   result_t *result = find_book(nodes,count,b1_id);
   if(result -> elements == NULL){
	   return result;
   }
    //get the one book
   book_t* thebook = result -> elements[0];
   free(result->elements);
   free(result);

   node_t* firstNode = malloc(sizeof(node_t));
   firstNode -> book = thebook;   
   firstNode -> level =0;  
   firstNode -> parent = NULL;
   firstNode -> prev = NULL;
   firstNode -> next = NULL;
      
  
   //result need be returned 
   result_t* path = malloc(sizeof(result_t));
   path->elements = NULL;
   path->n_elements = 0;
   path-> max = 10;

//this links is a node i created to make it easier to  free the other nodes
   size_t max =10;
   node_t** links = malloc(sizeof(node_t*)*max);
   links[0] = firstNode;
   size_t n_links=1; 

//    BFS(G, v)
//     Queue q;

   queue_t* queue = malloc(sizeof(queue_t));
   queue->head = firstNode;
   queue->last = firstNode;

//     for all vertices u in G
//         u.seen = false
 	for(size_t i = 0;i<count;i++){
	   nodes[i].is_visited = false;
   }
//     q.enque(v)
//     while(!q.empty())
	do{	
//         u = q.deque()
		node_t* currentNode = dequeue(queue);
	//if we find the book, we return
		if(currentNode->book->id == b2_id){
			node_t* temp = currentNode;
			do{
				path->n_elements++;
				if(path->elements==NULL){
					path->elements = malloc(sizeof(book_t*) * path->max);
				}
				if(path->n_elements==path->max){
					path->max = path->max << 1;
					path->elements = realloc(path->elements,sizeof(book_t*) * path->max);
				}
				path->elements[path->n_elements-1] = temp->book;
				temp = temp->parent;
			}while(temp != NULL);			
			break;
		}
//         if u.seen is false
//             u.seen = true

		if(currentNode->book->is_visited==false){
			currentNode->book->is_visited=true;
		
	//             for all neighbours w of u
			for(size_t i=0; i<currentNode->book->n_citation_edges;i++){
				size_t neighbour_id = currentNode->book->b_citation_edges[i];
				book_t* neighbour_book = nodes+neighbour_id;
				node_t* neighbour_node = malloc(sizeof(book_t));
				neighbour_node -> book = neighbour_book;
				neighbour_node -> parent = currentNode;
				neighbour_node->prev = NULL;
				neighbour_node->next = NULL;

		//                 q.enque(w)
				enqueue(queue,neighbour_node);

				if(n_links == max){
					max = max << 1;
					links = realloc(links,sizeof(node_t*)*max);
				}
				links[n_links] = neighbour_node;
				n_links++;
			}
			//author_edge
			for(size_t i=0; i<currentNode->book->n_author_edges;i++){
				size_t neighbour_id = currentNode->book->b_author_edges[i];
				book_t* neighbour_book = nodes+neighbour_id;
				node_t* neighbour_node = malloc(sizeof(book_t));
				neighbour_node -> book = neighbour_book;
				neighbour_node -> parent = currentNode;
				neighbour_node->prev = NULL;
				neighbour_node->next = NULL;

		//                 q.enque(w)
				enqueue(queue,neighbour_node);

				if(n_links == max){
					max = max << 1;
					links = realloc(links,sizeof(node_t*)*max);
				}
				links[n_links] = neighbour_node;
				n_links++;
			}
			//publisher_edge
			for(size_t i=0; i<currentNode->book->n_publisher_edges;i++){
				size_t neighbour_id = currentNode->book->b_publisher_edges[i];
				book_t* neighbour_book = nodes+neighbour_id;
				node_t* neighbour_node = malloc(sizeof(book_t));
				neighbour_node -> book = neighbour_book;
				neighbour_node -> parent = currentNode;
				neighbour_node->prev = NULL;
				neighbour_node->next = NULL;

		//                 q.enque(w)
				enqueue(queue,neighbour_node);

				if(n_links == max){
					max = max << 1;
					links = realloc(links,sizeof(node_t*)*max);
				}
				links[n_links] = neighbour_node;
				n_links++;
			}
		}	
	}while(!isEmpty(queue));
	for(size_t i=0;i<n_links;i++){
		free(links[i]);
	}
	free(links);
	free(queue);
	return path;	
}

// Returns result set containing books in shortest path of two edges types between author 1 and 2.
result_t* find_shortest_edge_type(book_t* nodes, size_t count, size_t a1_id, size_t a2_id) {

	// TODO
	return NULL;
}


