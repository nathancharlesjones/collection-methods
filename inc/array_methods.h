#ifndef ARRAY_METHODS_H
#define ARRAY_METHODS_H

#include <string.h>     // For memmove, memset
#include <stdbool.h>    // For bool

#define LEN_ARRAY(x) (sizeof(x)/sizeof(x[0]))

// Use the ARRAY_FOR_EACH macro to abbreviate iterating over an array.
//
// Useful for implementing a "map" operation (applies an operation to each element in an array), e.g.
//     int x[] = { 1, 2, 3, 4, 5 };
//
//     ARRAY_FOR_EACH(x, idx) x[idx] = x[idx]*x[idx];    // Squares each number in an array
//
//     ARRAY_FOR_EACH(x, idx)                            // Squares and then prints each number in an array
//     {
//         x[idx] = x[idx]*x[idx];
//         printf("%d, ", x[idx]);
//     }
//
// Also useful for implementing a "reduce" operation (calculates some scalar value over an array), e.g.
//     int x[] = { 1, 2, 3, 4, 5 }, sum;
//     float avg, SD = 0;
//
//     ARRAY_FOR_EACH(x, idx) sum += x[idx];                   // Sums the values in an array
//
//     avg = (float)sum / LEN_ARRAY(x);
//     ARRAY_FOR_EACH( x, idx ) SD += pow( (x[idx] - avg), 2); // Computes the standard deviation
//     SD = pow( (SD / LEN_ARRAY(x)), 0.5 );
//
#define ARRAY_FOR_EACH(ARRAY, INDEX) for( int INDEX = 0; INDEX < LEN_ARRAY(ARRAY); INDEX++ )

// For searching/sorting, use bsearch/qsort from stdlib
//     void* bsearch (const void* key, const void* base, size_t num, size_t size, int (*compar)(const void * key, const void * elem))
//     void qsort(void *base, size_t nitems, size_t size, int (*compar)(const void * elem1, const void * elem2))
//
// Those two functions, and the ones below, require 3 parameters, at a minimum, to be able to work on an array of arbitrary data:
//     - a pointer to the base of the array (of type "void *", so that any data type can be used),
//     - the total number of elements in the array ("num"), and
//     - the size of each element in the array in bytes.
// In this manner, each function can treat each array as just a collection of bytes and iterate over it or manipulate it without
// needing to know what the underlying data type is.
//
// Additionally, each function requires a function pointer which does the work of indicating whether two elements are out of order
// or if an item should be kept or counted.

// Linear search for unsorted arrrays. Returns the index of the value, if found, or -1 if not found (as **opposed** to "bsearch",
// which returns a pointer to the memory location of the found value).
//
static inline int array_find( const void * key, const void * base, size_t num, size_t size, int (*compare)(const void * key, const void * elem) )
{
    int ret = -1;

    for( int idx = 0; idx < num; idx++ )
    {
        const void * this_item = base + idx * size;
        if( 0 == compare( key, this_item ) )
        {
            ret = idx;
            break;
        }
    }

    return ret;
}

// Returns the index of the largest value in the array.
//
static inline int array_find_max( const void * base, size_t num, size_t size, int (*compare)(const void * item_one, const void * item_two) )
{
    int ret = 0;

    for( int idx = 0; idx < num; idx++ )
    {
        const void * this_item = base + idx * size;
        if( compare( &ret, this_item ) < 0 ) ret = idx;
    }

    return ret;
}

// Returns the index of the smallest value in the array.
//
static inline int array_find_min( const void * base, size_t num, size_t size, int (*compare)(const void * item_one, const void * item_two) )
{
    int ret = 0;

    for( int idx = 0; idx < num; idx++ )
    {
        const void * this_item = base + idx * size;
        if( compare( &ret, this_item ) > 0 ) ret = idx;
    }

    return ret;
}

// Keeps only those elements in an array for which the function "keep_this" returns "true", shifting elements
// to the left as needed. Zeros out any remaining part of the array. Returns the number of elements that were
// left in, for the purpose of allowing client code to initialize a new array that is just long enough
// to hold the filered values. Ex:
//
//     int x[] = {1,2,3,4,5};
//
//     static inline bool is_odd( const void * item )
//     {
//         return (*(int *)item) % 2 == 1;
//     }
//
//     array_filter(x, LEN_ARRAY(x), sizeof(int), is_odd);  // x is now [1,3,5,0,0]
// 
// Use the return value to initialize a new array whose length matches the filtered list. For example,
// if the line above which calls "array_filter" is changed to:
//
//     int new_size = array_filter(x, LEN_ARRAY(x), sizeof(int), is_odd);
//
// then we can create a new array with that length and copy over only the filtered values:
//
//     if( new_size > 0 )
//     {
//         int x_resized[new_size];
//         memcpy( x_resized, x, sizeof(int) * new_size);  // x_resized is now [1,3,5]
//     }
//     else
//     {
//         /* Handle empty array */
//     }
//
// Or, using dynamic memory:
//
//     if( new_size > 0 )
//     {
//         int * x_resized = calloc(1, sizeof(int) * new_size);
//         memcpy( x_resized, x, sizeof(int) * new_size);  // x_resized is now [1,3,5]
//     }
//     else
//     {
//         /* Handle empty array */
//     }
//
// **WARNING**: This function copies array elements to different locations in memory. Because of this, it will
// break any variables that hold pointers to any array elements, since those pointers will point to different
// pieces of data.
//
static inline int array_filter_in_place( void * base, size_t num, size_t size, bool (*keep_this)(const void * elem), void (*delete)(void * item) )
{
    void * end_of_filtered_array = base;
    const void * end_of_base_array = base + num * size;
    
    // (1) "this_item" and "end_of_filtered_array" both start out pointing to the first array element
    //
    for( void * this_item = base; this_item < end_of_base_array; this_item += size )
    {
        if( keep_this( this_item ) )
        {
            // (2) If this is an item we want to keep AND if "end_of_filtered_array" isn't already pointing to it,
            // then we need to copy it from it's current location to where "end_of_filtered_array" is pointing.
            // (If "end_of_filtered_array" is already pointing at the item we want to keep, then there's no need
            // to copy it onto itself.)
            //
            if( end_of_filtered_array != this_item )
            {
                memmove( end_of_filtered_array, this_item, size );
                memset( this_item, 0, size );
            }

            // (3) In either case, we can increment "end_of_filtered_array".
            //
            end_of_filtered_array += size;
        }
        else
        {
            // (4) If we _don't_ want to keep the item, we need to clear out the memory. But first, we check if
            // a "delete" function has been provided (to free dynamically allocated memory or deinitialize an
            // object), and call it, if so.
            //
            if( delete ) delete( this_item );
            memset( this_item, 0, size );
        }
    }

    return (end_of_filtered_array - base)/size;
}

// Copies to "filtered_array" only those elements in "base" for which the function "keep_this" returns "true".
// Does NOT modify the base array. Returns the number of elements that were left in.
//
// **WARNING**: This function _assumes_ that "filtered_array" is large enough to hold all of the filtered values.
// The only size guaranteed not to overflow is that which is equal to the "base" array, in case _all_ values
// meet the filter criteria. Using an array for "filtered_array" that is _smaller_ than "base" array incurs the
// risk of an overflow error, which this function can neither detect nor prevent!
//
static inline int array_filter_pure( const void * base, size_t num, size_t size, void * filtered_array, bool (*keep_this)(const void * elem) )
{
    int end_of_filtered_array = 0;
    const void * end_of_base_array = base + num * size;
    
    for( const void * this_item = base; this_item < end_of_base_array; this_item += size )
    {
        if( keep_this( this_item ) )
        {
            memcpy( filtered_array + end_of_filtered_array*size, this_item, size );
            end_of_filtered_array++;
        }
    }

    return end_of_filtered_array;
}

// Counts the number of items in the array for which the function "count_this" returns "true".
//
static inline int array_count( void * base, size_t num, size_t size, bool (*count_this)(const void * elem) )
{
    int count = 0;
    const void * end_of_base_array = base + num * size;
    
    for( const void * this_item = base; this_item < end_of_base_array; this_item += size )
    {
        if( count_this( this_item ) ) count++;
    }

    return count;
}

// Insert an element into the array at position "pos", shifting "pos" and all remaining elements to the right.
// Overwrites the last element in the array.
// 
// **WARNING**: This function copies array elements to different locations in memory. Because of this, it will
// break any variables that hold pointers to any array elements, since those pointers will point to different
// pieces of data.
//
static inline void array_insert( void * base, size_t num, size_t size, int pos, void * elem, void (*delete)(void * item) )
{
    void * last_item_in_base_array = base+size*(num-1);

    if( delete ) delete( last_item_in_base_array );
    for( int idx = num-1; idx > pos; idx-- ) memmove(base+size*idx, base+size*(idx-1), size);
    memcpy(base+size*pos, elem, size);
}

// Remove an element from the array at position "pos", shifting all remaining elements to the left. Clears out the
// last element in the array.
//
// **WARNING**: This function copies array elements to different locations in memory. Because of this, it will
// break any variables that hold pointers to any array elements, since those pointers will point to different
// pieces of data.
//
static inline void array_remove( void * base, size_t num, size_t size, int pos, void (*delete)(void * item) )
{
    if( delete ) delete(base+size*pos);
    for( int idx = pos; idx < num - 1; idx++ ) memmove(base+size*idx, base+size*(idx+1), size);
    memset( base+size*(num-1), 0, size);
}

// Byte-swap of a and b. Helper macro used by "array_reverse".
//
#define SWAP(a, b, size)                \
    do                                  \
    {                                   \
        size_t __size = (size);         \
        char * __a = (a), * __b = (b);  \
        do                              \
        {                               \
            char __tmp = * __a;         \
            * __a++ = * __b;            \
            * __b++ = __tmp;            \
        } while (--__size > 0);         \
    } while (0);    

// **WARNING**: This function copies array elements to different locations in memory. Because of this, it will
// break any variables that hold pointers to any array elements, since those pointers will point to different
// pieces of data.
//
static inline void array_reverse( void * base, size_t num, size_t size )
{
    for( int idx = 0; idx < num/2; idx++ ) SWAP( base+size*idx, base+size*(num-1-idx), size );
}

#endif // ARRAY_METHODS_H