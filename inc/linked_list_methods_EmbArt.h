#ifndef LINKED_LIST_METHODS_EMBART_H
#define LINKED_LIST_METHODS_EMBART_H

#include "ll.h"
#include <stdlib.h>     // For qsort
#include <stdbool.h>    // For bool

// Uses the [Embedded Artistry linked list](https://github.com/embeddedartistry/libmemory/blob/master/dependencies/lib/linkedlist/ll.h)
// implementation, which is a circular, doubly-linked list in which the head is an empty element that is part of the list.
//
// The implementation above provides "for_each" macros as well as functions to add, join, and delete nodes.

// Linear search for unsorted lists. Returns a pointer to the value, if found, or NULL if not found.
//
static inline void * linked_list_find( const void * key, const ll_t * head, int (*compare)(const void * key, const void * elem) )
{
    void * ret = NULL;
    ll_t * node;

    list_for_each( node, head )
    {
        if( 0 == compare( key, node ) )
        {
            ret = node;
            break;
        }
    }

    return ret;    
}

// Utilizes the function "qsort" from stdlib.h to sort a linked list. "qsort" expects to work on arrays, so first
// we build an array of pointers to each of the nodes in the linked list. These pointers are then sorted and the
// list is rebuilt using the array of sorted pointers. Requires a "compare" function which takes a _double-pointer_
// to the node type, since "qsort" will pass in a pointer to the array element which is, itself, a pointer to the
// node.
//
static inline void linked_list_qsort( ll_t * head, int (*compare)(const void * key, const void * elem) )
{
    ll_t * node;
    uint32_t count = 0;

    list_for_each( node, head ) count++;

    if( count > 0 )
    {
        ll_t * array_of_p_nodes[count];
        uint32_t idx = 0;
        list_for_each( node, head ) array_of_p_nodes[idx++] = node;
        qsort( array_of_p_nodes, count, sizeof(ll_t *), compare);
        ARRAY_FOR_EACH( array_of_p_nodes, idx )
        {
            list_del( array_of_p_nodes[idx] );
            list_add_tail( array_of_p_nodes[idx], head );
        }
    }
}

// Insert a node at the correct position in a sorted linked list. Helper function used by "linked_list_insertion_sort".
//
static inline void linked_list_sorted_insert( ll_t * head, ll_t * node_to_insert, int (*compare)(const void * key, const void * elem) )
{
    // Reference: https://www.geeksforgeeks.org/insertion-sort-for-singly-linked-list/

    ll_t * node;

    if( head->next == head || compare( head->next, node_to_insert ) > 0 )
    {
        list_add( node_to_insert, head );
    }
    else
    {
        list_for_each( node, head )
        {
            if( node->next == head || compare( node->next, node_to_insert ) > 0 )
            {
                list_insert( node_to_insert, node, node->next );
                break;
            }
        }
    }
}

// Basic insertion sort for linked lists.
//
static inline void linked_list_insertion_sort( ll_t * head, int (*compare)(const void * key, const void * elem) )
{
    ll_t *node, *copy;

    list_for_each( node, head )
    {
        if( node->prev == head ); // Skip the first element, since a list with only one element is already sorted
        else
        {
            if( compare( node->prev, node ) > 0 ) // If node < node->prev...
            {
                list_del( node );
                linked_list_sorted_insert( head, node, compare );
            }
        }
    }
}

//TODO: Add MergeSort (https://www.geeksforgeeks.org/iterative-merge-sort-for-linked-list/)

// Returns the address of the largest value in an unsorted linked list.
//
static inline void * linked_list_find_max( const ll_t * head, int (*compare)(const void * item_one, const void * item_two) )
{
    void * ret = (void *)head->next;
    ll_t * node;

    list_for_each( node, head )
    {
        if( compare(ret, node) < 0 ) ret = (void *)node;
    }

    return ret;
}

// Returns the address of the smallest value in an unsorted linked list.
//
static inline void * linked_list_find_min( const ll_t * head, int (*compare)(const void * item_one, const void * item_two) )
{
    void * ret = (void *)head->next;
    ll_t * node;

    list_for_each( node, head )
    {
        if( compare(ret, node) > 0 ) ret = (void *)node;
    }

    return ret;
}

// Keeps only those elements in a list for which the function "keep_this" returns "true", removing every other
// element and adding it to the list "removed_nodes" (if not NULL). Returns the number of elements that were
// removed, for the purpose of allowing client code to iterate over the removed nodes to deinitialize and/or
// free them.
//
static inline int linked_list_filter_in_place( ll_t * head, ll_t * removed_nodes, bool (*keep_this)(const void * elem) )
{
    int removed_count = 0;
    ll_t * node, * copy;

    list_for_each_safe( node, copy, head )
    {
        if( !keep_this( node ) )
        {
            list_del( node );
            if( removed_nodes ) list_add_tail( node, removed_nodes );
            removed_count++;
        }
    }

    return removed_count;
}

// Adds to "filtered_list" only those elements in the list that starts with "head" for which the function "keep_this"
// returns "true". Uses the function "copy_node" to create new nodes with the same data as the items to be kept and
// adds them to "filtered_list". Does NOT modify the list that starts with "head". Returns the number of elements
// that were added to "filtered_list".
//
static inline int linked_list_filter_pure( ll_t * head, ll_t * filtered_list, bool (*keep_this)(const void * elem), void * (*copy_node)(const void * elem) )
{
    int filtered_count = 0;
    ll_t * node;

    list_for_each( node, head )
    {
        if( keep_this( node ) )
        {
            void * copied_node = copy_node(node);
            list_add_tail( copied_node, filtered_list );
            filtered_count++;
        }
    }

    return filtered_count;
}

// Counts the number of items in the linked list for which the function "count_this" returns "true".
//
static inline int linked_list_count( const ll_t * head, bool (*count_this)(const void * elem) )
{
    int count = 0;
    ll_t * node;

    list_for_each( node, head )
    {
        if( count_this(node) ) count++;
    }

    return count;
}

static inline void linked_list_reverse( ll_t * head )
{
    ll_t *node, *copy, *temp;

    list_for_each_safe( node, copy, head )
    {
        temp = node->next;
        node->next = node->prev;
        node->prev = temp;
    }

    temp = head->next;
    head->next = head->prev;
    head->prev = temp;
}

#endif // LINKED_LIST_METHODS_EMBART_H