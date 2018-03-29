/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    /* Remember to handle the case if malloc returned NULL */
    queue_t *n = malloc(sizeof(queue_t));
    if(n == NULL) return NULL;
    n -> head = NULL;
    n -> tail = NULL;
    n -> size = 0;
    return n;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Remember to free the queue structure and list elements */
    if(q == NULL) return;
    list_ele_t *p = q -> head, *cur;
    while(p != NULL) {
        cur = p;
        p = p -> next;
        free(cur);
    }
    free(q);
    q -> tail = NULL; 
    q -> head = NULL; 
    q -> size = 0;
    q = NULL;
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_head(queue_t *q, int v)
{
    /* What should you do if the q is NULL? */
    /* What if malloc returned NULL? */
    if(q == NULL) return false;
    list_ele_t *new_ele = malloc(sizeof(list_ele_t));
    if(new_ele == NULL) return false;

    /* If list was empty before insert, update tail pointer */
    new_ele -> next = q -> head;
    new_ele -> value = v;
    q -> head = new_ele;
    if(q -> size == 0) q -> tail = new_ele;
    q -> size += 1;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
 */
bool q_insert_tail(queue_t *q, int v)
{
    /* Remember: It should operate in O(1) time */
    /* Return on q is NULL or malloc returned NULL */
    if(q == NULL) return false;
    list_ele_t *new_ele = malloc(sizeof(list_ele_t));
    if(new_ele == NULL) return false;

    /* If list was empty before insert, update head pointer */
    new_ele -> next = NULL;
    new_ele -> value = v;
    if(q -> tail == NULL) {
        q -> head = new_ele;
        q -> tail = new_ele;
    }
    else {
        q -> tail -> next = new_ele;
        q -> tail = new_ele;
    }
    q -> size += 1;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If vp non-NULL and element removed, store removed value at *vp.
  Any unused storage should be freed
*/
bool q_remove_head(queue_t *q, int *vp)
{
    if(q == NULL || q -> head == NULL) return false;
    list_ele_t *to_rem = q -> head;
    if(vp != NULL) *vp = to_rem -> value;
    q -> head = q -> head -> next;
    free(to_rem);
    q -> size -= 1;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* Remember: It should operate in O(1) time */
    if(q == NULL) return 0;
    return q -> size;
}

/*
  Reverse elements in queue.

  Your implementation must not allocate or free any elements (e.g., by
  calling q_insert_head or q_remove_head).  Instead, it should modify
  the pointers in the existing data structure.
 */
void q_reverse(queue_t *q)
{
    /* Return if list is NULL, empty or size=1 */ 
    if(q == NULL || q -> head == NULL || q -> head -> next == NULL) return;
    list_ele_t *old_head = q -> head;
    list_ele_t *old_tail = q -> tail;
    list_ele_t *a = q -> head;
    list_ele_t *b = q -> head -> next;
    list_ele_t *c = q -> head -> next -> next;

    b -> next = a;
    a -> next = NULL;
    while(c != NULL) {
        a = b;
        b = c;
        c = c -> next;
        b -> next = a;
    }
    q -> head = old_tail;
    q -> tail = old_head;
}

