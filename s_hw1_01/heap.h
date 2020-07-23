#ifndef HEAP_H
#define HEAP_H


//To Use

// Step 1 Create what you want for a node this means making a struct for it 
// Step 2 Create a comparator for that struct.
// Step 3 Use heap_init( &heap, (whatever you name comparator) , NULL)
// Step 4 Initialize a node with all the variables you want/need. 
// Step 5 Shove that node into you heap using heap_insert( &heap, (whatever you name your node))
// Step 6 Repeat 4 and 5 until you are satisfied.

// To remove the top (smallest cost) node use: heap_remove_min(&heap))
// (I'm still trying to comprehend it, but this is a list of what I'm understanding so far)




#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

  /**
   * Heap node struct
   *  //forward declaration, defined in heap.c
   * */
  struct heap_node;

  /**
   * Heap node struct type
   * */
  typedef struct heap_node heap_node_t;

  /**
   * Heap struct and type
   * */
  typedef struct heap
  {
    heap_node_t *min;
    uint32_t size;
    int32_t (*compare)(const void *key, const void *with);
    void (*datum_delete)(void *);
  } heap_t;

  /**
   * Inits a heap
   * Takes a new heap struct
   * 
   * */
  void heap_init(heap_t *h, int32_t (*compare)(const void *key, const void *with), void (*datum_delete)(void *));

  /**
   * Deletes a heap/frees
   * */
  void heap_delete(heap_t *h);

  /**
   * 
   * @returns heap_node_t
   * */
  heap_node_t *heap_insert(heap_t *h, void *v);

  /**
   * 
   * */
  void *heap_peek_min(heap_t *h);

  /**
   * Removes the smallest cost node //may return it but not sure yet
   * */
  void *heap_remove_min(heap_t *h);

  /**
   * probably not using
   * */
  int heap_combine(heap_t *h, heap_t *h1, heap_t *h2);

  /**
   * 
   * */
  int heap_decrease_key(heap_t *h, heap_node_t *n, void *v);

  /**
   * 
   * */
  int heap_decrease_key_no_replace(heap_t *h, heap_node_t *n);



#ifdef __cplusplus
}
#endif
#endif
