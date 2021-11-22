# Thursday, 20 May 2021
- Arrays need to iterate over indices. Linked lists need to iterate over values.
- Not sure if my implementation (specifically the do...while part) will allow collections to be created
- Don't like that some argument (cmp, inc, blk) require parentheses but iter doesn't

# Friday, 21 May 2021
- Not liking the macro implementation. It's hella powerful, but I'm not understanding when I can/can't more than simple statements.
    - Couldn't seem to include "if" statement in OPERATION. Got around it using ternary and modulo.
- Can't figure out how to interate through linked list fully; end condition always seems to leave out last node. In other words, the linked list macros only work for linked lists with a dummy node to indicate the end of the list.
    - Actually, I could do this just fine for a normal linked list by using the fact that the last "next" is NULL.
    - Could maybe do this with a do..while loop, but it feels sufficiently niche that I can probably leave it off. It makes sense that a circularly linked list might need to have a "dummy" node to indicate the tail.

# Saturday, 22 May 2021
- Slight annoyance that I can't return a value from a macro without GCC statement expressions
- Problem with variable shadowing, especially since "idx" (and variants) are common variable names when using generic iterators
- Hmm, I'm using "typeof" in my LINKED_LIST_FOR_EACH implementation, which is a GCC extension. Do I care?
    - No, but only becuause it's a [proposed addition to the next major C Standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2685.htm)

# Tuesday, 25 May 2021
- Need to change `ARRAY_FIND` and `ARRAY_FIND_MAX` macros; both assume they're operating on scalar types that support simple `==` and `>` operations. This doesn't work for floats or structs.
- In fact, cumbersome as it might be, I think the better implementation is to use functions, following the example of `bsearch` and `qsort` from `stdlib.h`.
    - `void* bsearch (const void* key, const void* base, size_t num, size_t size, int (*compar)(const void * key, const void * elem))`
        - Returns a pointer to the spot in the array where `key` was found, or `NULL`
        - `compar` should return:
            - <0 if key comes before elem,
            - 0 if key is equivalent to elem, or
            - >0 if key comes after elem.
    - `void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void * elem1, const void * elem2))`
        - In-place sort
        - `compar` same as above
    - For arrays, need to pass in three things:
        - (1) a `void *` pointer to the array,
        - (2) the size of each element in the array, and
        - (3) the number of elements in the array.
    - For linked lists, need to pass in ...three? things:
        - (1) a `void *` pointer to the head of the linked list,
        - (2) a function to get the next item from the list,
        - (3) something to determine when the end of the list is reached?

# Wednesday, 26 May 2021
- Using functions feels much more natural and harder to mess up. Macros have the convenience of knowing the type and size of an array, but the downside of not easily being able to return values (without compiler extensions) and possibly not being able to handle arbitrarily complex comparison functions.
- Other collection methods:
    - count
    - group by?
    - insert? (insert_shoft_right, insert_shift_left)
    - pop? (pop_shift_right, pop_shift_left)
- Demo "For each...if something...do something" -> list.filter().map()
- Should any functions change back to macros? "count" and "filter" _do_ just require Boolean functions. The expressions for those are probably pretty simple.
    - Nah, I can imagine some not-too-weird logic that just wouldn't work with a macro (or wouldn't work cleanly).
- I should try to refactor my functions.

# Thursday, 27 May 2021
- I should add tests and clean up the demo in main.
- Possible to redefine the FOR_EACH macros so that they're more useful to my functions?
- Add LinkedList class.
- Does "linked list excluded" work for "linked list included"??
    - They do work both ways, but there are weird "off by 1" errors, since "included" expects 1 empty value but "excluded" doesn't. So a list that _excludes_ head will have one element that gets missed when the "included" logic is used. A list that _includes_ head will have one extra element processed (the "head" element) when the "excluded" logic is used.
- Change filter/pop for user to put in "cleared" value?
- SERIOUSLY. I NEED TESTS.

# Monday, 07 June 2021
- [Array operations](https://www.boost.org/doc/libs/1_73_0/libs/preprocessor/doc/AppendixA-AnIntroductiontoPreprocessorMetaprogramming.html) (from the Boost preprocessor)
    - Insert
    - Pop back
    - Pop front
    - Push back
    - Push front
    - Remove
    - Replace
    - Reverse
- Some operations not needed or superfluous:
    - Pop back/front: Can't return a value. If the goal is to remove the front/back element, just use remove.
    - Push back: Might be useful, but there's not an easy way to know that an array of size N only has M (< N) elements. Use insert instead.
    - Push front: Use insert.
    - Replace: Use normal C array syntax (i.e. `array[pos] = new_val`).
- Added tests (yay!).
- Added "delete" function to "array_filter", "array_insert", and "array_remove", to account for the possibility that array elements may need to be "free"'d or deinitialized.
    - Need to add tests for this.

# Wednesday, 09 June 2021
- Decided to use Phil's LL implementation to start
- Couldn't think of an easy way to iterate over the list nodes without `container_of`/`offset_of` macros, so I kept with `void *`.
- Still not sure I like the macro implementation
- Stuck on linked_list_filter, since I'm having trouble figuring out how to inform the calling code of the removed nodes. Filter part works fine, though.
    - Fixed by making `removed_nodes` an out-argument.
- It'll be a bit more complicated, but I'm starting to like the idea of a LL ADT. It will clean up a lot of the code at the minor expense of moving struct and function definitions to separate source/header files from the calling code.

# Thursday, 10 June 2021
- How to implement `linked_list_filter_pure`? Either the `filtered_nodes` argument needs to be a fully-formed list, to which the _data_ of the filtered nodes gets copied or the function needs to be able to create new nodes to attach to the list `filtered_nodes`. The latter feels more... stable?
- If I'm able to create new nodes, do I need the arguments `removed_nodes` and `filtered_nodes`?
    - I think so, otherwise the filter function would need a separate function pointer for creating/initializing a linked list.

# Friday, 11 June 2021
- Interesting thought: If I ever feel the need to debug some code or add printfs after a test fails, that's an indicator that my code has outrun my tests.
- I think I'm finished with the basic linked list extensions! All except for a MergeSort implementation. I'm feeling _pretty_ good about them, though I'm fairly certain that at several points I'm tying the code to the fact that I'm working with a circular, doubly-linked list with a head element included in the list.
- Test list is getting rather long, so I tried to add the test runners. Makefile getting mad at me, though.
- TODO: Fix the fact that the makefile requires a source file for every "Test..." file.

# References
- [Oracle LL implementation](https://www.oracle.com/solaris/technologies/c-type.html)
- [Linux LL implementation](https://www.davidespataro.it/kernel-linux-list-efficient-generic-linked-list/)
- [Embedded Artistry LL implementation](https://github.com/embeddedartistry/libmemory/blob/master/dependencies/lib/linkedlist/ll.h)
- [bsearch](https://www.cplusplus.com/reference/cstdlib/bsearch/)
- [qsort](https://www.cplusplus.com/reference/cstdlib/qsort/)
- [Data Structure and Algorithms - Linked List](https://www.tutorialspoint.com/data_structures_algorithms/linked_list_algorithms.htm) (TutorialsPoint.com)