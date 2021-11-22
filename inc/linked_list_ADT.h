#ifndef LINKED_LIST_H
#define LINEKD_LIST_H

// -----What is a linked list?-----
// A linked list is a data structure that includes a pointer to another data structure of the same type. In the graphic
// below, each box represents a variable in a given piece of code; all but one of the boxes is a "node" or "element" in
// our linked list. "Head" is a variable which is a pointer to a "node". More specifically, it's a pointer to the first
// node in our list. Each node contains a piece of data, here represented by the letters "A", "B", and "C"; the data
// type can be any valid data type. Each node also has an element called "next" which points to the next item in the list.
// The value of "next" for the last item in the list is NULL.
//     ┌──────┐    ┌───┬──────┐    ┌───┬──────┐    ┌───┬──────┐
//     │ Head ├───►│ A │ next ├───►│ B │ next ├───►│ C │ NULL │
//     └──────┘    └───┴──────┘    └───┴──────┘    └───┴──────┘
//
// In C, we might create the nodes for a linked list like this:
//     typedef struct node_t
//     {
//         int                data;
//         struct node_t *    next;
//     } node_t;
//
// If A.data = 1, B.data = 2, and C.data = 3 in the example above, then we could create our linked list in the following manner:
//     node_t C = { 3, NULL };
//     node_t B = { 2, &C };
//     node_t A = { 1, &B };
//     node_t * Head = &A;
//
// Linked lists are sort of like "dynamic arrays", in that their size and the order of their nodes can easily change over
// their lifetime. Changing the size or order of arrays is much more difficult than for linked lists. Although you lose
// the ability to directly reference each node (instead, you must traverse the list to find the node you want), you gain
// the ability to easily insert, remove, and reorder the items in a linked list.

// -----Types of linked lists-----
// The linked list shown above has three qualities:
//     - it's linear, in that it doesn't wrap around on itself,
//     - it's singly-linked, meaning that each node only contains a single pointer to the next node, and
//     - the "head" element, our entry into the linked list, is defined as a pointer to the first node.
//
// Linked lists can also have the following qualities:
//     - circular, in that the last element points back to the first element,
//     - doubly-linked, in which each node also contains a pointer to the previous node, or
//     - the "head" element can be an empty element in the linked list.
//
// These linked lists might look like the graphics below (the first linked list shown above is repeated
// below for the purposes of comparison).
//
//     - Linear, singly-linked list where "head" is a pointer to the first node
//
//         ┌──────┐    ┌───┬──────┐    ┌───┬──────┐    ┌───┬──────┐
//         │ Head ├───►│ A │ next ├───►│ B │ next ├───►│ C │ NULL │
//         └──────┘    └───┴──────┘    └───┴──────┘    └───┴──────┘
//
//     - Linear, doubly-linked where "head" is a pointer to the first node
//
//         ┌──────┐    ┌──────┬───┬─────┐◄───┬──────┬───┬──────┐◄───┬──────┬───┬──────┐
//         │ Head ├───►│ NULL │ A │ next│    │ prev │ B │ next │    │ prev │ C │ NULL │
//         └──────┘    └──────┴───┴─────┴───►└──────┴───┴──────┴───►└──────┴───┴──────┘
//
//     - Linear, singly-linked list where "head" is an empty element in the list
//
//         /-----HEAD-----\
//         ┌───────┬──────┐    ┌───┬──────┐    ┌───┬──────┐    ┌───┬──────┐
//         │ Empty │ next ├───►│ A │ next ├───►│ B │ next ├───►│ C │ NULL │
//         └───────┴──────┘    └───┴──────┘    └───┴──────┘    └───┴──────┘
//
//     - Linear, doubly-linked where "head" is an empty element in the list
//
//         /-----HEAD-----\
//         ┌───────┬──────┐◄───┬──────┬───┬─────┐◄───┬──────┬───┬──────┐◄───┬──────┬───┬──────┐
//         │ Empty │ next │    │ prev │ A │ next│    │ prev │ B │ next │    │ prev │ C │ NULL │
//         └───────┴──────┴───►└──────┴───┴─────┴───►└──────┴───┴──────┴───►└──────┴───┴──────┘
//
//     - Circular, singly-linked lists where "head" is a pointer to the first node
//
//         ┌──────┐    ┌───┬──────┐    ┌───┬──────┐    ┌───┬──────┐
//         │ Head ├───►│ A │ next ├───►│ B │ next ├───►│ C │ next │
//         └──────┘    └───┴──────┘    └───┴──────┘    └───┴────┬─┘
//                       ▲                                      │
//                       └──────────────────────────────────────┘
//     - Circular, doubly-linked lists where "head" is a pointer to the first node
//
//         ┌──────┐    ┌──────┬───┬─────┐◄───┬──────┬───┬──────┐◄───┬──────┬───┬──────┐
//         │ Head ├───►│ prev │ A │ next│    │ prev │ B │ next │    │ prev │ C │ next │
//         └──────┘    └────┬─┴───┴─────┴───►└──────┴───┴──────┴───►└──────┴───┴───┬──┘
//                       ▲  │                                         ▲            │
//                       │  └─────────────────────────────────────────┘            │
//                       │                                                         │
//                       └─────────────────────────────────────────────────────────┘
//
//     - Circular, singly-linked lists where "head" is an empty element in the list
//
//         /-----HEAD-----\
//         ┌───────┬──────┐    ┌───┬──────┐    ┌───┬──────┐    ┌───┬──────┐
//         │ Empty │ next ├───►│ A │ next ├───►│ B │ next ├───►│ C │ next │
//         └───────┴──────┘    └───┴──────┘    └───┴──────┘    └───┴────┬─┘
//           ▲                                                          │
//           └──────────────────────────────────────────────────────────┘
//
//     - Circular, doubly-linked lists where "head" is an empty element in the list
//
//         /---------HEAD--------\
//         ┌──────┬───────┬──────┐◄───┬──────┬───┬──────┐◄───┬──────┬───┬──────┐◄───┬──────┬───┬──────┐
//         │ prev │ Empty │ next │    │ prev │ A │ next │    │ prev │ B │ next │    │ prev │ C │ next │
//         └─┬────┴───────┴──────┴───►└──────┴───┴──────┴───►└──────┴───┴──────┴───►└──────┴───┴─┬────┘
//           │  ▲                                                                                │  ▲
//           │  └────────────────────────────────────────────────────────────────────────────────┘  │
//           │                                                                                      │
//           └──────────────────────────────────────────────────────────────────────────────────────┘
//
// Circular linked lists map well to things like queues and circular buffers which, by nature, exhibit a "wrapping" characteristic.
//
// Doubly-linked lists make traversal easier, since we can move backward through the list if we need to.
//
// Having the "head" element be an actual item in the linked list wastes a data element but offers two benefits:
//     1) It makes it easier to modify the head element of the linked list (since we can easily modify the value of `head->next`).
//     2) It makes one full traversal of the list slightly more straightforward for circularly linked lists. (Circular linked
//     lists where "head" is a pointer to the first node have to iterate until the first node is _returned_ to. This requires 
//     some sort of do...while loop or a flag variable to allow the traversing code to execute at least once for the first node 
//     but no more. For circular linked lists where "head" is an empty element in the list, the code merely needs to stop when 
//     the node containing "head" is reached.)


// The logic for these two types
// of circular lists is quite different, which is why there are two types of the LINKED_LIST_FOR_EACH macro below: one 
// for use with the first four linked lists (where "Head" is excluded from the list) and another for use with the last 
// two types of linked lists (where "Head" is included as an item in the list). A substitution macro, LINKED_LIST_FOR_EACH, 
// is provided so you don't have to type out the much more verbose names for those two macros.


// -----References-----
// - [Data Structure and Algorithms - Linked List](https://www.tutorialspoint.com/data_structures_algorithms/linked_list_algorithms.htm) (TutorialsPoint.com)
// - [Linked List Data Structure](https://www.geeksforgeeks.org/data-structures/linked-list/) (GeeksForGeeks.com)
// - Linked list implementations
//     - [Oracle LL implementation](https://www.oracle.com/solaris/technologies/c-type.html)
//     - [Linux LL implementation](https://www.davidespataro.it/kernel-linux-list-efficient-generic-linked-list/)
//     - [Embedded Artistry LL implementation](https://github.com/embeddedartistry/libmemory/blob/master/dependencies/lib/linkedlist/ll.h)

#endif // LINKED_LIST_H