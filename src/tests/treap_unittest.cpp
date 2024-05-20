#include "treaplib/treap.h"
#include "gtest/gtest.h"

TEST(TreapTest, InsertTest) {
  Treap<int> treap;
  treap.insert(5);
  treap.insert(3);
  treap.insert(8);
  treap.insert(2);
  treap.insert(4);
  treap.insert(7);
  treap.insert(9);
  EXPECT_TRUE(treap.search(5));
  EXPECT_FALSE(treap.search(6));
}

TEST(TreapTest, EraseTest) {
  Treap<int> treap;
  treap.insert(5);
  treap.insert(3);
  treap.insert(8);
  treap.insert(2);
  treap.insert(4);
  treap.insert(7);
  treap.insert(9);
  treap.erase(3);

  EXPECT_FALSE(treap.search(3));
  EXPECT_TRUE(treap.search(5));
}

TEST(TreapTest, SearchTest) {
  Treap<int> treap;
  treap.insert(5);
  treap.insert(3);
  treap.insert(8);
  treap.insert(2);
  treap.insert(4);
  treap.insert(7);
  treap.insert(9);
  EXPECT_TRUE(treap.search(5));
  EXPECT_FALSE(treap.search(6));
}

TEST(TreapCopyConstructorTest, CopyConstructorWorks) {
  Treap<int> treap1;
  treap1.insert(5);
  treap1.insert(10);
  treap1.insert(3);

  Treap<int> treap2 = treap1;

  EXPECT_TRUE(treap2.search(5));
  EXPECT_TRUE(treap2.search(10));
  EXPECT_TRUE(treap2.search(3));
}

TEST(TreapCopyAssignmentTest, CopyAssignmentOperatorWorks) {
  Treap<int> treap1;
  treap1.insert(5);
  treap1.insert(10);
  treap1.insert(3);

  Treap<int> treap2;
  treap2.insert(20);
  treap2.insert(15);

  treap2 = treap1;

  EXPECT_TRUE(treap2.search(5));
  EXPECT_TRUE(treap2.search(10));
  EXPECT_TRUE(treap2.search(3));
  EXPECT_FALSE(treap2.search(20));
  EXPECT_FALSE(treap2.search(15));
}

TEST(TreapTest, MoveConstructor) {
  Treap<int> t1;
  t1.insert(5);
  t1.insert(10);
  t1.insert(3);

  Treap<int> t2(std::move(t1));

  // t1 should be empty after moving its resources to t2
  EXPECT_FALSE(t1.search(5));
  EXPECT_FALSE(t1.search(10));
  EXPECT_FALSE(t1.search(3));

  // t2 should have the same elements as t1 had
  EXPECT_TRUE(t2.search(5));
  EXPECT_TRUE(t2.search(10));
  EXPECT_TRUE(t2.search(3));
}

TEST(TreapTest, MoveAssignmentOperator) {
  Treap<int> t1;
  t1.insert(5);
  t1.insert(10);
  t1.insert(3);

  Treap<int> t2;
  t2.insert(20);
  t2.insert(15);
  t2.insert(8);

  t2 = std::move(t1);

  // t1 should be empty after moving its resources to t2
  EXPECT_FALSE(t1.search(5));
  EXPECT_FALSE(t1.search(10));
  EXPECT_FALSE(t1.search(3));

  // t2 should have the same elements as t1 had
  EXPECT_TRUE(t2.search(5));
  EXPECT_TRUE(t2.search(10));
  EXPECT_TRUE(t2.search(3));
}

TEST(TreapTest, InsertAndIterator) {
  Treap<int> t;
  t.insert(3);
  t.insert(1);
  t.insert(4);
  t.insert(1); // Duplicates are ignored
  t.insert(2);

  std::vector<int> expected_values = {1, 2, 3, 4};
  std::vector<int> actual_values;

  for (const auto &value : t) {
    actual_values.push_back(value);
  }

  EXPECT_EQ(expected_values, actual_values);
}

// Example test for erasing elements and checking iterators
TEST(TreapTest, EraseAndIterator) {
  Treap<int> t;
  t.insert(3);
  t.insert(1);
  t.insert(4);
  t.insert(2);

  t.erase(3);
  t.erase(1);

  std::vector<int> expected_values = {2, 4};
  std::vector<int> actual_values;

  for (const auto &value : t) {
    actual_values.push_back(value);
  }

  EXPECT_EQ(expected_values, actual_values);
}

// Example test for checking an empty tree
TEST(TreapTest, EmptyTree) {
  Treap<int> t;
  std::vector<int> expected_values = {};
  std::vector<int> actual_values;

  for (const auto &value : t) {
    actual_values.push_back(value);
  }

  EXPECT_EQ(expected_values, actual_values);
}