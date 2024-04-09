#include <gtest.h>
#include "ordered_linkedlist.h"

TEST(OrderedLinkedList, default_comparator_sorts_by_ascending)
{
    OrderedLinkedList<int> list;
    list.push(5);
    list.push(1);
    list.push(2);
    list.push(4);
    list.push(3);

    ASSERT_EQ(5, list.size());

    EXPECT_EQ(1, list[0]);
    EXPECT_EQ(2, list[1]);
    EXPECT_EQ(3, list[2]);
    EXPECT_EQ(4, list[3]);
    EXPECT_EQ(5, list[4]);
}

TEST(OrderedLinkedList, custom_comparator_sorts_by_descending)
{
    OrderedLinkedList<int> list([](const auto& a, const auto& b) { return a > b; });
    list.push(5);
    list.push(1);
    list.push(2);
    list.push(4);
    list.push(3);

    ASSERT_EQ(5, list.size());

    EXPECT_EQ(5, list[0]);
    EXPECT_EQ(4, list[1]);
    EXPECT_EQ(3, list[2]);
    EXPECT_EQ(2, list[3]);
    EXPECT_EQ(1, list[4]);
}

TEST(OrderedLinkedList, can_copy)
{
    OrderedLinkedList<int> list;
    list.push(5);
    list.push(1);
    list.push(2);
    list.push(4);
    list.push(3);

    OrderedLinkedList<int> copied = list;

    ASSERT_EQ(list.size(), copied.size());
    for (int i = 0; i < list.size(); i++) {
        EXPECT_EQ(list[i], copied[i]);
    }
}

TEST(OrderedLinkedList, can_copy_assign)
{
    OrderedLinkedList<int> list;
    list.push(5);
    list.push(1);
    list.push(2);
    list.push(4);
    list.push(3);

    OrderedLinkedList<int> dst;
    dst = list;

    ASSERT_EQ(list.size(), dst.size());
    for (int i = 0; i < list.size(); i++)
    {
        EXPECT_EQ(list[i], dst[i]);
    }
}

TEST(OrderedLinkedList, can_compare)
{
    OrderedLinkedList<int> lhs;
    lhs.push(1);
    lhs.push(2);

    OrderedLinkedList<int> rhs;
    rhs.push(1);
    rhs.push(2);
    rhs.push(3);

    EXPECT_EQ(false, lhs == rhs);

    lhs.push(3);

    EXPECT_EQ(true, lhs == rhs);

    rhs.remove(2);

    EXPECT_EQ(false, lhs == rhs);
}
