#include <stdbool.h>
#include <stdlib.h>
//#include "unity_fixture.h"
#include "unity.h"
#include "array_methods.h"
#include "linked_list_methods_EmbArt.h"
#include "ll.h"

uint32_t actual[5];

typedef struct myStruct_t
{
    ll_t        node;
    uint32_t    data;
} myStruct_t;

LIST_INIT(myList);
myStruct_t node_A = {.data = 1};
myStruct_t node_B = {.data = 2};
myStruct_t node_C = {.data = 3};
myStruct_t node_D = {.data = 4};
myStruct_t node_E = {.data = 5};

LIST_INIT(myList_unsorted);
myStruct_t node_F = {.data = 1};
myStruct_t node_G = {.data = 2};
myStruct_t node_H = {.data = 3};
myStruct_t node_I = {.data = 4};
myStruct_t node_J = {.data = 5};

myStruct_t node_K, node_L;

void setUp(void)
{
    ARRAY_FOR_EACH(actual, idx) actual[idx] = idx + 1;

    list_add_tail(&node_A.node, &myList);
    list_add_tail(&node_B.node, &myList);
    list_add_tail(&node_C.node, &myList);
    list_add_tail(&node_D.node, &myList);
    list_add_tail(&node_E.node, &myList);
    
    list_add_tail( &node_I.node, &myList_unsorted );
    list_add_tail( &node_G.node, &myList_unsorted );
    list_add_tail( &node_H.node, &myList_unsorted );
    list_add_tail( &node_J.node, &myList_unsorted );
    list_add_tail( &node_F.node, &myList_unsorted );
}

void tearDown(void)
{
    ll_t *node, *copy;
    list_for_each_safe( node, copy, &myList) list_del(node);
    list_for_each_safe( node, copy, &myList_unsorted) list_del(node);
}

static inline bool is_odd( const void * item )
{
    return (*(int *)item) % 2 == 1;
}

static inline bool greater_than_ten( const void * item )
{
    return (*(int *)item) > 10;
}

static inline bool is_odd_myStruct( const void * item )
{
    return ((myStruct_t *)item)->data % 2 == 1;
}

static inline int compare_myStructs( const void * item_one, const void * item_two )
{
    return ((myStruct_t *)item_one)->data - ((myStruct_t *)item_two)->data;
}

static inline int compare_myStructs_for_qsort( const void * item_one, const void * item_two )
{
    return (*(myStruct_t **)item_one)->data - (*(myStruct_t **)item_two)->data;
}

static inline void * copy_node_myStruct( const void * elem )
{
    myStruct_t * new = calloc(1, sizeof(myStruct_t));
    new->data = ((myStruct_t *)elem)->data;
    return new;
}

void test_array_filter_in_place(void)
{
    uint32_t expected[] = {1,3,5,0,0};
    uint32_t num_filtered = array_filter_in_place(actual, LEN_ARRAY(actual), sizeof(actual[0]), is_odd, NULL);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
    TEST_ASSERT_EQUAL_UINT32( 3, num_filtered );
}

void test_array_filter_in_place_returns_zero_for_no_items_kept(void)
{
    uint32_t expected[] = {0,0,0,0,0};
    uint32_t num_filtered = array_filter_in_place(actual, LEN_ARRAY(actual), sizeof(actual[0]), greater_than_ten, NULL);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
    TEST_ASSERT_EQUAL_UINT32( 0, num_filtered );
}

void test_array_filter_pure(void)
{
    uint32_t initial[] = {6,7,8,9,10};
    ARRAY_FOR_EACH(actual, idx) actual[idx] = 0;
    uint32_t expected[] = {7,9,0,0,0};
    uint32_t num_filtered = array_filter_pure(initial, LEN_ARRAY(initial), sizeof(initial[0]), &actual, is_odd);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
    TEST_ASSERT_EQUAL_UINT32( 2, num_filtered );
}

void test_array_filter_pure_returns_zero_for_no_items_kept(void)
{
    uint32_t initial[] = {6,7,8,9,10};
    ARRAY_FOR_EACH(actual, idx) actual[idx] = 0;
    uint32_t expected[] = {0,0,0,0,0};
    uint32_t num_filtered = array_filter_pure(initial, LEN_ARRAY(initial), sizeof(initial[0]), &actual, greater_than_ten);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
    TEST_ASSERT_EQUAL_UINT32( 0, num_filtered );
}

void test_array_insert(void)
{
    uint32_t expected[] = {1,2,9,3,4};
    uint32_t elem_to_insert = 9;
    array_insert(actual, LEN_ARRAY(actual), sizeof(actual[0]), 2, &elem_to_insert, NULL);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
}

void test_array_insert_at_end(void)
{
    uint32_t expected[] = {1,2,3,4,9};
    uint32_t elem_to_insert = 9;
    array_insert(actual, LEN_ARRAY(actual), sizeof(actual[0]), 4, &elem_to_insert, NULL);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
}

void test_array_remove(void)
{
    uint32_t expected[] = {1,2,4,5,0};
    array_remove(actual, LEN_ARRAY(actual), sizeof(actual[0]), 2, NULL);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
}

void test_array_remove_from_end(void)
{
    uint32_t expected[] = {1,2,3,4,0};
    array_remove(actual, LEN_ARRAY(actual), sizeof(actual[0]), 4, NULL);
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
}

void test_array_reverse(void)
{
    uint32_t expected[] = {5,4,3,2,1};
    array_reverse(actual, LEN_ARRAY(actual), sizeof(actual[0]));
    TEST_ASSERT_EQUAL_UINT32_ARRAY(expected, actual, LEN_ARRAY(actual));
}

void test_linked_list_find(void)
{
    myStruct_t key = {.data = 4};
    myStruct_t * found = (myStruct_t *)linked_list_find( &key, &myList, compare_myStructs );
    TEST_ASSERT_TRUE(found == &node_D);
    TEST_ASSERT_EQUAL_UINT32(key.data, found->data);
}

void test_linked_list_find_max(void)
{
    myStruct_t * found = (myStruct_t *)linked_list_find_max( &myList, compare_myStructs );
    TEST_ASSERT_TRUE(found == &node_E);
    TEST_ASSERT_EQUAL_UINT32(node_E.data, found->data);
}

void test_linked_list_find_min(void)
{
    myStruct_t * found = (myStruct_t *)linked_list_find_min( &myList, compare_myStructs );
    TEST_ASSERT_TRUE(found == &node_A);
    TEST_ASSERT_EQUAL_UINT32(node_A.data, found->data);
}

void test_linked_list_count(void)
{
    int count = linked_list_count( &myList, is_odd_myStruct );
    TEST_ASSERT_EQUAL( 3, count );
}

void test_linked_list_filter_in_place(void)
{
    LIST_INIT(removed_nodes);
    int count = linked_list_filter_in_place( &myList, &removed_nodes, is_odd_myStruct );
    uint32_t idx = 0, expected_remaining[] = {1,3,5};
    ll_t *node;
    TEST_ASSERT_EQUAL( 2, count );
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected_remaining[idx++], ((myStruct_t *)node)->data);
    }
    idx = 0;
    uint32_t expected_removed[] = {2,4};
    list_for_each( node, &removed_nodes )
    {
        TEST_ASSERT_EQUAL_UINT32( expected_removed[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_filter_pure(void)
{
    LIST_INIT(filtered_nodes);
    int count = linked_list_filter_pure( &myList, &filtered_nodes, is_odd_myStruct, copy_node_myStruct );
    uint32_t idx = 0, expected_remaining[] = {1,2,3,4,5};
    ll_t *node;
    TEST_ASSERT_EQUAL( 3, count );
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected_remaining[idx++], ((myStruct_t *)node)->data);
    }
    idx = 0;
    uint32_t expected_filtered[] = {1,3,5};
    list_for_each( node, &filtered_nodes )
    {
        TEST_ASSERT_EQUAL_UINT32( expected_filtered[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_qsort(void)
{
    ll_t *node;
    linked_list_qsort( &myList_unsorted, compare_myStructs_for_qsort );
    uint32_t idx = 0, expected[] = {1,2,3,4,5};
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_sorted_insert_adds_to_tail(void)
{
    node_K.data = 6;
    linked_list_sorted_insert( &myList, &node_K.node, compare_myStructs );
    ll_t *node;
    uint32_t idx = 0, expected[] = {1,2,3,4,5,6};
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
    TEST_ASSERT_EQUAL( 6, idx );
}

void test_linked_list_sorted_insert_adds_to_head(void)
{
    node_K.data = 0;
    linked_list_sorted_insert( &myList, &node_K.node, compare_myStructs );
    ll_t *node;
    uint32_t idx = 0, expected[] = {0,1,2,3,4,5};
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_sorted_insert_adds_to_empty_list(void)
{
    LIST_INIT(test_list);
    TEST_ASSERT_TRUE( test_list.next == &test_list );
    node_K.data = 1;
    linked_list_sorted_insert( &test_list, &node_K.node, compare_myStructs );
    ll_t *node;
    uint32_t idx = 0, expected[] = {1};
    TEST_ASSERT_TRUE( test_list.next != &test_list );
    list_for_each( node, &test_list )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_sorted_insert_adds_to_middle(void)
{
    node_K.data = 10;
    list_add_tail( &node_K.node, &myList );
    ll_t *node;
    uint32_t idx = 0, expected[7] = {1,2,3,4,5,10};
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
    node_L.data = 8;
    linked_list_sorted_insert( &myList, &node_L.node, compare_myStructs );
    idx = 0;
    expected[5] = 8, expected[6] = 10; // expected = {1,2,3,4,5,8,10}
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_insertion_sort(void)
{
    ll_t *node;
    list_for_each( node, &myList_unsorted ) printf("%16d\t%s", ((myStruct_t *)node)->data, node == (ll_t *)(&node_F) ? "\n" : "");
    list_for_each( node, &myList_unsorted ) printf("%16p\t%s", node, node == (ll_t *)(&node_F) ? "\n" : "");
    linked_list_insertion_sort( &myList_unsorted, compare_myStructs );
    uint32_t idx = 0, expected[] = {1,2,3,4,5};
    list_for_each( node, &myList_unsorted )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
}

void test_linked_list_reverse(void)
{
    linked_list_reverse( &myList );
    uint32_t idx = 0, expected[] = {5,4,3,2,1};
    ll_t *node;
    list_for_each( node, &myList )
    {
        TEST_ASSERT_EQUAL_UINT32( expected[idx++], ((myStruct_t *)node)->data);
    }
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_array_filter_in_place);
    RUN_TEST(test_array_filter_in_place_returns_zero_for_no_items_kept);
    RUN_TEST(test_array_filter_pure);
    RUN_TEST(test_array_filter_pure_returns_zero_for_no_items_kept);
    RUN_TEST(test_array_insert);
    RUN_TEST(test_array_insert_at_end);
    RUN_TEST(test_array_remove);
    RUN_TEST(test_array_remove_from_end);
    RUN_TEST(test_array_reverse);
    RUN_TEST(test_linked_list_find);
    RUN_TEST(test_linked_list_find_max);
    RUN_TEST(test_linked_list_find_min);
    RUN_TEST(test_linked_list_count);
    RUN_TEST(test_linked_list_filter_in_place);
    RUN_TEST(test_linked_list_filter_pure);
    RUN_TEST(test_linked_list_qsort);
    RUN_TEST(test_linked_list_sorted_insert_adds_to_tail);
    RUN_TEST(test_linked_list_sorted_insert_adds_to_head);
    RUN_TEST(test_linked_list_sorted_insert_adds_to_empty_list);
    RUN_TEST(test_linked_list_sorted_insert_adds_to_middle);
    RUN_TEST(test_linked_list_insertion_sort);
    RUN_TEST(test_linked_list_reverse);
    return UNITY_END();
}