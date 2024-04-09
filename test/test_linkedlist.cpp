#include <gtest.h>
#include "linkedlist.h"

TEST(LinkedList, can_push_back)
{
    const int element = 5;

    LinkedList<int> list;
    list.push_back(element + 1);
    list.push_back(element);

    EXPECT_EQ(element, list.back());
    EXPECT_EQ(element, list[list.size() - 1]);
    EXPECT_EQ(element + 1, list[list.size() - 2]);
}

TEST(LinkedList, can_push_front)
{
    const int element = 5;

    LinkedList<int> list;
    list.push_front(element + 1);
    list.push_front(element);

    EXPECT_EQ(element, list.front());
    EXPECT_EQ(element, list[0]);
    EXPECT_EQ(element + 1, list[1]);
}

TEST(LinkedList, can_insert)
{
    const int element = 10;
    const int values[] = { 1, 2, 3, 4, 5 };

    LinkedList<int> list;
    for (const int& value : values) {
        list.push_back(value);
    }
    list.insert(2, element);

    EXPECT_EQ(element, list[2]);
}

TEST(LinkedList, can_access_by_index)
{
    const int values[] = { 1, 2, 3, 4, 5 };
    const size_t length = sizeof(values) / sizeof(*values);

    LinkedList<int> list;
    for (const int& value : values) {
        list.push_back(value);
    }

    ASSERT_EQ(length, list.size());

    for (size_t i = 0; i < length; i++) {
        EXPECT_EQ(values[i], list[i]);
    }
}

TEST(LinkedList, can_assign_by_index)
{
    const int src_values[] = { 1, 2, 3, 4, 5 };
    const size_t length = sizeof(src_values) / sizeof(*src_values);

    int dst_values[length] = { 6, 7, 8, 9, 10 };

    LinkedList<int> list;
    for (const int& value : src_values) {
        list.push_back(value);
    }

    ASSERT_EQ(length, list.size());

    for (size_t i = 0; i < length; i++) {
        list[i] = dst_values[i];
    }

    for (size_t i = 0; i < length; i++) {
        EXPECT_EQ(dst_values[i], list[i]);
    }
}

TEST(LinkedList, can_copy)
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    LinkedList<int> copied = list;

    ASSERT_EQ(list.size(), copied.size());
    for (int i = 0; i < list.size(); i++) {
        EXPECT_EQ(list[i], copied[i]);
    }
}

TEST(LinkedList, can_copy_assign)
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    LinkedList<int> dst;
    dst = list;

    ASSERT_EQ(list.size(), dst.size());
    for (int i = 0; i < list.size(); i++)
    {
        EXPECT_EQ(list[i], dst[i]);
    }
}

TEST(LinkedList, can_move)
{
    const int values[] = { 1, 2, 3, 4, 5 };
    const size_t length = sizeof(values) / sizeof(*values);

    LinkedList<int> list;
    for (const int& value : values) {
        list.push_back(value);
    }
    ASSERT_EQ(list.size(), length);

    LinkedList<int> moved = std::move(list);
    EXPECT_EQ(length, moved.size());

    for (int i = 0; i < length; i++)
    {
        EXPECT_EQ(moved[i], values[i]);
    }
}

TEST(LinkedList, can_compare)
{
    LinkedList<int> lhs;
    lhs.push_back(1);
    lhs.push_back(2);

    LinkedList<int> rhs;
    rhs.push_back(1);
    rhs.push_back(2);
    rhs.push_back(3);

    EXPECT_EQ(false, lhs == rhs);

    lhs.push_back(3);

    EXPECT_EQ(true, lhs == rhs);

    rhs.remove(2);

    EXPECT_EQ(false, lhs == rhs);
}

TEST(LinkedList, copy_is_equal_to_source)
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    LinkedList<int> copied = list;

    EXPECT_EQ(list, copied);
}

TEST(LinkedList, copy_assigned_is_equal_to_source)
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    LinkedList<int> copied;
    copied = list;

    EXPECT_EQ(list, copied);
}

TEST(LinkedList, zero_length_list_is_empty)
{
    LinkedList<int> list;
    ASSERT_EQ(0, list.size());
    EXPECT_EQ(true, list.empty());

    list.push_back(1);
    EXPECT_EQ(false, list.empty());

    list.remove(0);
    EXPECT_EQ(true, list.empty());
}

TEST(LinkedList, access_amortizer_cache_correctness)
{
    LinkedList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    list.push_back(5);

    int _ = list[2];
    list.insert(2, 10);

    EXPECT_EQ(10, list[2]);
    EXPECT_EQ(4, list[4]);

    list.remove(3);

    EXPECT_EQ(5, list[4]);
}

TEST(LinkedList, iterator)
{
    int values[] = { 1, 2, 3, 4, 5 };
    const size_t length = sizeof(values) / sizeof(*values);

    LinkedList<int> list;
    for (const int& value : values) {
        list.push_back(value);
    }

    int* p = values;
    size_t idx = 0;
    for (const auto& item : list) {
        EXPECT_EQ(*p++, item);
        idx++;
    }

    EXPECT_EQ(length, idx);
}
