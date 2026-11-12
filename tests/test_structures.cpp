#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include "../structures/array.h"
#include "../structures/slist.h"
#include "../structures/dlist.h"
#include "../structures/stack.h"
#include "../structures/queue.h"
#include "../structures/hashSet.h"
#include "../structures/cuckooHash.h"
#include "../structures/blackredtree.h"

// ================= ARRAY =================
TEST(ArrayTest, BasicOperations) {
    Array arr(2);
    EXPECT_EQ(arr.length(), 0);
    EXPECT_EQ(arr.get(0), "");

    arr.addEnd("A");
    arr.addEnd("B");
    EXPECT_EQ(arr.length(), 2);

    arr.addIndex("C", 1);
    EXPECT_EQ(arr.get(1), "C");
    EXPECT_EQ(arr.get(2), "B");

    arr.addIndex("D", 10);

    arr.removeIndex(1);
    EXPECT_EQ(arr.get(1), "B");

    arr.removeIndex(10);

    arr.set(0, "X");
    EXPECT_EQ(arr.get(0), "X");

    arr.set(10, "Y");
}

TEST(ArrayTest, Resize) {
    Array arr(1);
    arr.addEnd("1");
    arr.addEnd("2");
    arr.addEnd("3");
    EXPECT_EQ(arr.length(), 3);
}

TEST(ArrayTest, InvalidIndexes) {
    Array arr(1);
    EXPECT_EQ(arr.get(10), "");
    arr.set(10, "X");
    EXPECT_EQ(arr.get(0), "");

    arr.addIndex("Y", 10);
    arr.removeIndex(10);
    EXPECT_EQ(arr.length(), 0);
}

TEST(ArrayTest, PrintAndEmpty) {
    Array arr;
    arr.print();
    arr.addEnd("Test");
    arr.print();
}

TEST(ArrayTest, BinarySerialization) {
    Array arr;
    arr.addEnd("Alpha");
    arr.addEnd("Beta");
    arr.addEnd("Gamma");

    const string file = "test_array.bin";
    ASSERT_TRUE(arr.saveBinary(file));

    Array loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_EQ(loaded.length(), 3);
    EXPECT_EQ(loaded.get(0), "Alpha");
    EXPECT_EQ(loaded.get(1), "Beta");
    EXPECT_EQ(loaded.get(2), "Gamma");

    Array loaded2;
    loaded2.addEnd("Old");
    ASSERT_TRUE(loaded2.loadBinary(file));
    EXPECT_EQ(loaded2.length(), 3);
    EXPECT_EQ(loaded2.get(0), "Alpha");

    Array empty;
    ASSERT_TRUE(empty.saveBinary(file));
    Array loadedEmpty;
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_EQ(loadedEmpty.length(), 0);

    EXPECT_FALSE(arr.loadBinary("no_such_file.bin"));

    std::remove(file.c_str());
}

// ================= SLIST =================
TEST(SlistTest, PushAndFind) {
    Slist list;
    EXPECT_FALSE(list.find("A"));

    list.pushHead("B");
    list.pushHead("A");
    list.pushTail("C");
    list.pushAfter("B", "D");
    list.pushBefore("B", "E");

    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("D"));
    EXPECT_TRUE(list.find("E"));

    list.deleteHead();
    list.deleteTail();
    list.deleteAfter("E");
    list.deleteBefore("E");
    list.deleteValue("E");
    list.deleteValue("NonExistent");
}

TEST(SlistTest, EdgeCases) {
    Slist list;
    list.deleteHead();
    list.deleteTail();
    list.pushBefore("NonExistent", "X");
}

TEST(SlistTest, NotExistBranches) {
    Slist list;
    list.pushHead("A");
    list.deleteBefore("B");
    list.deleteAfter("B");
    list.pushAfter("B", "C");
    list.pushBefore("B", "D");
}

TEST(SlistTest, FullCoverage) {
    Slist list;
    list.pushHead("A");
    list.pushTail("B");
    list.pushAfter("A", "C");
    list.pushBefore("B", "D");
    list.print();
    list.clear();
    EXPECT_EQ(list.find("A"), false);
}

TEST(SlistTest, BinarySerialization) {
    Slist list;
    list.pushTail("one");
    list.pushTail("two");
    list.pushTail("three");

    const string file = "test_slist.bin";
    ASSERT_TRUE(list.saveBinary(file));

    Slist loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_TRUE(loaded.find("one"));
    EXPECT_TRUE(loaded.find("two"));
    EXPECT_TRUE(loaded.find("three"));

    Slist loaded2;
    loaded2.pushTail("old1");
    loaded2.pushTail("old2");
    ASSERT_TRUE(loaded2.loadBinary(file));
    EXPECT_FALSE(loaded2.find("old1"));
    EXPECT_TRUE(loaded2.find("one"));

    Slist empty;
    ASSERT_TRUE(empty.saveBinary(file));
    Slist loadedEmpty;
    loadedEmpty.pushHead("X");
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_FALSE(loadedEmpty.find("X"));

    EXPECT_FALSE(list.loadBinary("no_such_file.bin"));
    std::remove(file.c_str());
}

TEST(SlistTest, PushHeadIntoSingleNode) {
    Slist list;
    list.pushHead("M");
    list.pushHead("A");
    list.pushHead("Z");
    EXPECT_TRUE(list.find("Z"));
    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("M"));
}

TEST(SlistTest, DeleteHeadSequential) {
    Slist list;
    list.pushTail("1");
    list.pushTail("2");
    list.pushTail("3");
    list.pushTail("4");

    list.deleteHead();
    EXPECT_FALSE(list.find("1"));
    EXPECT_TRUE(list.find("2"));

    list.deleteHead();
    EXPECT_FALSE(list.find("2"));
    EXPECT_TRUE(list.find("3"));

    list.deleteHead();
    list.deleteHead();
    EXPECT_FALSE(list.find("3"));
    EXPECT_FALSE(list.find("4"));

    list.deleteHead();
    EXPECT_FALSE(list.find("4"));
}

TEST(SlistTest, DeleteTailSequential) {
    Slist list;
    list.pushTail("a");
    list.pushTail("b");
    list.pushTail("c");
    list.pushTail("d");

    list.deleteTail();
    EXPECT_FALSE(list.find("d"));
    EXPECT_TRUE(list.find("c"));

    list.deleteTail();
    EXPECT_FALSE(list.find("c"));
    EXPECT_TRUE(list.find("b"));

    list.deleteTail();
    list.deleteTail();
    EXPECT_FALSE(list.find("a"));

    list.deleteTail();
    EXPECT_FALSE(list.find("a"));
}

TEST(SlistTest, DeleteAfterMiddleAndTail) {
    Slist list;
    list.pushTail("A");
    list.pushTail("B");
    list.pushTail("C");
    list.pushTail("D");

    list.deleteAfter("B");
    EXPECT_FALSE(list.find("C"));
    EXPECT_TRUE(list.find("D"));

    list.deleteAfter("A");
    EXPECT_FALSE(list.find("B"));
    EXPECT_TRUE(list.find("D"));

    list.deleteAfter("D");
    EXPECT_TRUE(list.find("D"));
}

TEST(SlistTest, DeleteBeforeMiddleAndHead) {
    Slist list;
    list.pushTail("A");
    list.pushTail("B");
    list.pushTail("C");
    list.pushTail("D");

    list.deleteBefore("C");
    EXPECT_FALSE(list.find("B"));
    EXPECT_TRUE(list.find("A"));

    list.deleteBefore("D");
    EXPECT_FALSE(list.find("C"));
    EXPECT_TRUE(list.find("A"));

    list.deleteBefore("A");
    EXPECT_TRUE(list.find("A"));
}

TEST(SlistTest, DeleteValueFirstOccurrenceOnly) {
    Slist list;
    list.pushTail("A");
    list.pushTail("B");
    list.pushTail("C");
    list.pushTail("B");
    list.pushTail("D");

    list.deleteValue("B");
    EXPECT_TRUE(list.find("B"));
    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("D"));

    list.deleteValue("B");
    EXPECT_FALSE(list.find("B"));

    list.deleteValue("A");
    EXPECT_FALSE(list.find("A"));

    list.deleteValue("D");
    EXPECT_FALSE(list.find("D"));

    list.deleteValue("C");
    EXPECT_FALSE(list.find("C"));
}

TEST(SlistTest, ClearTwice) {
    Slist list;
    list.pushTail("A");
    list.pushTail("B");
    list.clear();
    EXPECT_FALSE(list.find("A"));
    list.clear();
    EXPECT_FALSE(list.find("B"));
}

TEST(SlistTest, RepeatedPushAfterSameValue) {
    Slist list;
    list.pushHead("X");
    list.pushAfter("X", "Y");
    list.pushAfter("X", "Z");
    list.pushAfter("Y", "W");

    EXPECT_TRUE(list.find("Y"));
    EXPECT_TRUE(list.find("Z"));
    EXPECT_TRUE(list.find("W"));
}

TEST(SlistTest, PushBeforeHead) {
    Slist list;
    list.pushHead("B");
    list.pushHead("C");
    list.pushBefore("C", "A");
    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("B"));
    EXPECT_TRUE(list.find("C"));
}

TEST(SlistTest, PushTailToEmpty) {
    Slist list;
    list.pushTail("only");
    EXPECT_TRUE(list.find("only"));
    list.deleteTail();
    EXPECT_FALSE(list.find("only"));
}

TEST(SlistTest, SerializationOrderPreserved) {
    Slist list;
    list.pushTail("1");
    list.pushTail("2");
    list.pushTail("3");
    list.pushTail("4");

    const string file = "test_slist_order.bin";
    ASSERT_TRUE(list.saveBinary(file));

    Slist loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_TRUE(loaded.find("1"));
    EXPECT_TRUE(loaded.find("2"));
    EXPECT_TRUE(loaded.find("3"));
    EXPECT_TRUE(loaded.find("4"));

    std::remove(file.c_str());
}

TEST(SlistTest, SerializationAfterDeletions) {
    Slist list;
    list.pushTail("a");
    list.pushTail("b");
    list.pushTail("c");
    list.deleteValue("b");

    const string file = "test_slist_del.bin";
    ASSERT_TRUE(list.saveBinary(file));

    Slist loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_TRUE(loaded.find("a"));
    EXPECT_FALSE(loaded.find("b"));
    EXPECT_TRUE(loaded.find("c"));

    std::remove(file.c_str());
}

// ================= DLIST =================
TEST(DlistTest, PushAndDelete) {
    Dlist list;
    list.pushTail("B");
    list.pushHead("A");
    list.pushTail("C");
    list.pushAfter("B", "D");
    list.pushBefore("B", "E");

    EXPECT_TRUE(list.find("D"));
    EXPECT_TRUE(list.find("E"));

    list.deleteHead();
    list.deleteTail();
    list.deleteAfter("E");
    list.deleteBefore("E");
    list.deleteValue("E");
    list.deleteValue("NonExistent");

    list.printForward();
    list.printBackward();
}

TEST(DlistTest, EdgeCases) {
    Dlist list;
    list.deleteHead();
    list.deleteTail();
    list.pushAfter("Non", "X");
}

TEST(DlistTest, PrintCoverage) {
    Dlist list;
    list.printForward();
    list.printBackward();
    list.pushTail("A");
    list.pushHead("B");
    list.printForward();
    list.printBackward();
}

TEST(DlistTest, ExtremeCoverage) {
    Dlist list;
    list.pushHead("A");
    list.pushTail("B");
    list.pushAfter("A", "C");
    list.pushBefore("B", "D");

    list.deleteAfter("A");
    list.deleteBefore("B");
    list.deleteValue("A");
    list.deleteValue("B");

    list.printForward();
    list.printBackward();
}

TEST(DlistTest, BinarySerialization) {
    Dlist list;
    list.pushTail("first");
    list.pushTail("second");
    list.pushTail("third");

    const string file = "test_dlist.bin";
    ASSERT_TRUE(list.saveBinary(file));

    Dlist loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_TRUE(loaded.find("first"));
    EXPECT_TRUE(loaded.find("second"));
    EXPECT_TRUE(loaded.find("third"));

    Dlist loaded2;
    loaded2.pushHead("old");
    ASSERT_TRUE(loaded2.loadBinary(file));
    EXPECT_FALSE(loaded2.find("old"));
    EXPECT_TRUE(loaded2.find("first"));

    Dlist empty;
    ASSERT_TRUE(empty.saveBinary(file));
    Dlist loadedEmpty;
    loadedEmpty.pushHead("X");
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_FALSE(loadedEmpty.find("X"));

    EXPECT_FALSE(list.loadBinary("no_such_file.bin"));
    std::remove(file.c_str());
}

// ================= STACK =================
TEST(StackTest, PushAndPop) {
    Stack stack;
    EXPECT_TRUE(stack.isEmpty());

    stack.push("1");
    stack.push("2");
    EXPECT_FALSE(stack.isEmpty());
    stack.print();

    std::string val;
    stack.pop(val);
    EXPECT_EQ(val, "2");

    stack.pop(val);
    EXPECT_EQ(val, "1");

    stack.pop(val);
    EXPECT_TRUE(stack.isEmpty());
}

TEST(StackTest, EmptyPop) {
    Stack s;
    std::string val;
    s.pop(val);
    EXPECT_TRUE(s.isEmpty());
}

TEST(StackTest, BinarySerialization) {
    Stack stack;
    stack.push("bottom");
    stack.push("middle");
    stack.push("top");

    const string file = "test_stack.bin";
    ASSERT_TRUE(stack.saveBinary(file));

    Stack loaded;
    ASSERT_TRUE(loaded.loadBinary(file));

    std::string val;
    loaded.pop(val);
    EXPECT_EQ(val, "top");
    loaded.pop(val);
    EXPECT_EQ(val, "middle");
    loaded.pop(val);
    EXPECT_EQ(val, "bottom");
    EXPECT_TRUE(loaded.isEmpty());

    Stack loaded2;
    loaded2.push("old");
    ASSERT_TRUE(loaded2.loadBinary(file));
    loaded2.pop(val);
    EXPECT_EQ(val, "top");
    loaded2.pop(val);
    EXPECT_EQ(val, "middle");
    loaded2.pop(val);
    EXPECT_EQ(val, "bottom");

    Stack empty;
    ASSERT_TRUE(empty.saveBinary(file));
    Stack loadedEmpty;
    loadedEmpty.push("X");
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_TRUE(loadedEmpty.isEmpty());

    EXPECT_FALSE(stack.loadBinary("no_such_file.bin"));
    std::remove(file.c_str());
}

// ================= QUEUE =================
TEST(QueueTest, PushAndPop) {
    Queue queue;
    EXPECT_TRUE(queue.isEmpty());

    queue.push("1");
    queue.push("2");
    EXPECT_FALSE(queue.isEmpty());
    queue.print();

    queue.pop();
    queue.pop();
    queue.pop();
    EXPECT_TRUE(queue.isEmpty());
}

TEST(QueueStackTest, EmptyPop) {
    Queue q;
    q.pop();
    EXPECT_TRUE(q.isEmpty());

    Stack s;
    std::string val;
    s.pop(val);
    EXPECT_TRUE(s.isEmpty());
}

TEST(QueueTest, BinarySerialization) {
    Queue queue;
    queue.push("a");
    queue.push("b");
    queue.push("c");

    const string file = "test_queue.bin";
    ASSERT_TRUE(queue.saveBinary(file));

    Queue loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_FALSE(loaded.isEmpty());

    loaded.pop();
    loaded.pop();
    loaded.pop();
    EXPECT_TRUE(loaded.isEmpty());

    Queue loaded2;
    loaded2.push("old");
    ASSERT_TRUE(loaded2.loadBinary(file));
    loaded2.pop(); loaded2.pop(); loaded2.pop();
    EXPECT_TRUE(loaded2.isEmpty());

    Queue empty;
    ASSERT_TRUE(empty.saveBinary(file));
    Queue loadedEmpty;
    loadedEmpty.push("X");
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_TRUE(loadedEmpty.isEmpty());

    EXPECT_FALSE(queue.loadBinary("no_such_file.bin"));
    std::remove(file.c_str());
}

// ================= HASHSET =================
TEST(HashSetTest, AddRemoveContains) {
    HashSet<std::string> set;
    EXPECT_FALSE(set.contains("apple"));

    set.add("apple");
    set.add("banana");
    set.add("apple");
    EXPECT_EQ(set.getSize(), 2);

    std::string* all = set.getAll();
    EXPECT_TRUE(all != nullptr);
    delete[] all;

    set.remove("apple");
    EXPECT_FALSE(set.contains("apple"));
    EXPECT_TRUE(set.remove("banana"));
    EXPECT_FALSE(set.remove("NonExistent"));
    EXPECT_EQ(set.getSize(), 0);
}

TEST(HashSetTest, ClearAndGetAll) {
    HashSet<std::string> set;
    set.add("1");
    set.add("2");
    set.add("3");

    std::string* all = set.getAll();
    EXPECT_TRUE(all != nullptr);
    delete[] all;

    set.free();
    EXPECT_EQ(set.getSize(), 0);
    EXPECT_FALSE(set.contains("1"));
}

TEST(HashSetTest, ForceCollisions) {
    HashSet<std::string> set(2);
    set.add("A");
    set.add("B");
    set.add("C");
    set.add("D");

    EXPECT_TRUE(set.contains("C"));
    EXPECT_TRUE(set.remove("B"));
    EXPECT_TRUE(set.remove("C"));
}

TEST(HashSetTest, BinarySerialization) {
    HashSet<std::string> set;
    set.add("alpha");
    set.add("beta");
    set.add("gamma");

    const string file = "test_hashset.bin";
    ASSERT_TRUE(set.saveBinary(file));

    HashSet<std::string> loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_TRUE(loaded.contains("alpha"));
    EXPECT_TRUE(loaded.contains("beta"));
    EXPECT_TRUE(loaded.contains("gamma"));
    EXPECT_EQ(loaded.getSize(), 3);

    HashSet<std::string> loaded2;
    loaded2.add("old");
    ASSERT_TRUE(loaded2.loadBinary(file));
    EXPECT_FALSE(loaded2.contains("old"));
    EXPECT_TRUE(loaded2.contains("alpha"));
    EXPECT_EQ(loaded2.getSize(), 3);

    HashSet<std::string> empty;
    ASSERT_TRUE(empty.saveBinary(file));
    HashSet<std::string> loadedEmpty;
    loadedEmpty.add("X");
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_EQ(loadedEmpty.getSize(), 0);

    EXPECT_FALSE(set.loadBinary("no_such_file.bin"));
    std::remove(file.c_str());
}

// ================= CUCKOOHASH =================
TEST(CuckooHashTest, AddAndContains) {
    CuckooHash<std::string> ch(4);
    EXPECT_FALSE(ch.contains("key1"));

    ch.add("key1");
    ch.add("key2");
    ch.add("key3");
    ch.add("key4");
    ch.add("key5");
    ch.add("key1");

    EXPECT_TRUE(ch.contains("key1"));
    EXPECT_TRUE(ch.contains("key5"));
    EXPECT_FALSE(ch.contains("key6"));
}

TEST(CuckooHashTest, ForceRehash) {
    CuckooHash<std::string> ch(2);
    for (int i = 0; i < 50; ++i) {
        ch.add("key_" + std::to_string(i));
    }
    EXPECT_TRUE(ch.contains("key_0"));
    EXPECT_TRUE(ch.contains("key_49"));
}

TEST(CuckooHashTest, EmptyContains) {
    CuckooHash<std::string> ch(2);
    EXPECT_FALSE(ch.contains("nothing"));
}

TEST(CuckooHashTest, BinarySerialization) {
    CuckooHash<std::string> ch(4);
    ch.add("one");
    ch.add("two");
    ch.add("three");

    const string file = "test_cuckoo.bin";
    ASSERT_TRUE(ch.saveBinary(file));

    CuckooHash<std::string> loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    EXPECT_TRUE(loaded.contains("one"));
    EXPECT_TRUE(loaded.contains("two"));
    EXPECT_TRUE(loaded.contains("three"));

    CuckooHash<std::string> loaded2;
    loaded2.add("old");
    ASSERT_TRUE(loaded2.loadBinary(file));
    EXPECT_FALSE(loaded2.contains("old"));
    EXPECT_TRUE(loaded2.contains("one"));

    CuckooHash<std::string> empty;
    ASSERT_TRUE(empty.saveBinary(file));
    CuckooHash<std::string> loadedEmpty;
    loadedEmpty.add("X");
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_FALSE(loadedEmpty.contains("X"));

    EXPECT_FALSE(ch.loadBinary("no_such_file.bin"));
    std::remove(file.c_str());
}

// ================= RED BLACK TREE =================
TEST(RedBlackTreeTest, InsertAndSearch) {
    RedBlackTree tree;
    EXPECT_FALSE(tree.search(10));

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);

    EXPECT_TRUE(tree.search(10));
    EXPECT_TRUE(tree.search(30));
    EXPECT_FALSE(tree.search(100));

    tree.print();
}

TEST(RedBlackTreeTest, DuplicateInsertKeepsValue) {
    RedBlackTree tree;
    tree.insert(10);
    tree.insert(10);
    EXPECT_TRUE(tree.search(10));

    tree.remove(10);
    EXPECT_TRUE(tree.search(10));

    tree.remove(10);
    EXPECT_FALSE(tree.search(10));
}

TEST(RedBlackTreeTest, Traversals) {
    RedBlackTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    tree.print();
    tree.clear();
    EXPECT_FALSE(tree.search(10));
}

TEST(RedBlackTreeTest, InsertZigZag) {
    RedBlackTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(15);
    tree.insert(5);
    tree.insert(1);
    tree.insert(8);

    EXPECT_TRUE(tree.search(15));
}

TEST(RedBlackTreeTest, SortedAscendingInsert) {
    RedBlackTree tree;
    for (int i = 1; i <= 64; ++i) tree.insert(i);
    for (int i = 1; i <= 64; ++i) EXPECT_TRUE(tree.search(i));
    EXPECT_FALSE(tree.search(0));
    EXPECT_FALSE(tree.search(65));
}

TEST(RedBlackTreeTest, SortedDescendingInsert) {
    RedBlackTree tree;
    for (int i = 64; i >= 1; --i) tree.insert(i);
    for (int i = 1; i <= 64; ++i) EXPECT_TRUE(tree.search(i));
    EXPECT_FALSE(tree.search(0));
    EXPECT_FALSE(tree.search(65));
}

TEST(RedBlackTreeTest, AllNegativeAndZero) {
    RedBlackTree tree;
    std::vector<int> values = {-50, -30, -70, 0, -20, -40, -60, -80};
    for (int v : values) tree.insert(v);
    for (int v : values) EXPECT_TRUE(tree.search(v));
    EXPECT_FALSE(tree.search(1));
    EXPECT_FALSE(tree.search(-1));

    tree.remove(0);
    EXPECT_FALSE(tree.search(0));
    tree.remove(-80);
    EXPECT_FALSE(tree.search(-80));
    for (int v : values) {
        if (v != 0 && v != -80) EXPECT_TRUE(tree.search(v));
    }
}

TEST(RedBlackTreeTest, RemoveNonExistentValues) {
    RedBlackTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.remove(0);
    tree.remove(15);
    tree.remove(25);
    tree.remove(100);

    EXPECT_TRUE(tree.search(10));
    EXPECT_TRUE(tree.search(20));
    EXPECT_TRUE(tree.search(30));
}

TEST(RedBlackTreeTest, RemoveFromEmpty) {
    RedBlackTree tree;
    tree.remove(1);
    EXPECT_FALSE(tree.search(1));
}

TEST(RedBlackTreeTest, RemoveSingleNode) {
    RedBlackTree tree;
    tree.insert(42);
    tree.remove(42);
    EXPECT_FALSE(tree.search(42));

    tree.remove(42);
    EXPECT_FALSE(tree.search(42));
}

TEST(RedBlackTreeTest, ClearOnEmpty) {
    RedBlackTree tree;
    tree.clear();
    EXPECT_FALSE(tree.search(1));

    tree.insert(1);
    tree.clear();
    EXPECT_FALSE(tree.search(1));
    tree.clear();
    EXPECT_FALSE(tree.search(1));
}

TEST(RedBlackTreeTest, PrintOnEmptyAndFilled) {
    RedBlackTree tree;
    tree.print();
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.print();
    tree.clear();
    tree.print();
}

TEST(RedBlackTreeTest, BinarySerialization) {
    RedBlackTree tree;
    std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    for (int v : values) tree.insert(v);

    const char* file = "test_rbtree.bin";
    ASSERT_TRUE(tree.saveBinary(file));

    RedBlackTree loaded;
    ASSERT_TRUE(loaded.loadBinary(file));

    for (int v : values) {
        EXPECT_TRUE(loaded.search(v));
    }
    EXPECT_FALSE(loaded.search(9999));

    RedBlackTree loaded2;
    loaded2.insert(1234);
    ASSERT_TRUE(loaded2.loadBinary(file));
    EXPECT_FALSE(loaded2.search(1234));
    EXPECT_TRUE(loaded2.search(50));
    for (int v : values) EXPECT_TRUE(loaded2.search(v));

    RedBlackTree empty;
    ASSERT_TRUE(empty.saveBinary(file));
    RedBlackTree loadedEmpty;
    loadedEmpty.insert(7);
    ASSERT_TRUE(loadedEmpty.loadBinary(file));
    EXPECT_FALSE(loadedEmpty.search(7));
    EXPECT_FALSE(loadedEmpty.search(50));

    EXPECT_FALSE(tree.loadBinary("no_such_file.bin"));
    std::remove(file);
}

TEST(RedBlackTreeTest, SerializationLargeTree) {
    RedBlackTree tree;
    std::mt19937 rng(12345);
    std::vector<int> values;
    for (int i = 1; i <= 100; ++i) values.push_back(i);
    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values) tree.insert(v);

    const char* file = "test_rbtree_large.bin";
    ASSERT_TRUE(tree.saveBinary(file));

    RedBlackTree loaded;
    ASSERT_TRUE(loaded.loadBinary(file));
    for (int i = 1; i <= 100; ++i) EXPECT_TRUE(loaded.search(i));
    EXPECT_FALSE(loaded.search(0));
    EXPECT_FALSE(loaded.search(101));

    std::remove(file);
}

TEST(RedBlackTreeTest, RemoveLeafNodes) {
    RedBlackTree tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);
    tree.insert(15);
    tree.insert(25);
    tree.insert(35);

    tree.remove(5);
    tree.remove(15);
    tree.remove(25);
    tree.remove(35);

    EXPECT_FALSE(tree.search(5));
    EXPECT_FALSE(tree.search(15));
    EXPECT_FALSE(tree.search(25));
    EXPECT_FALSE(tree.search(35));
    EXPECT_TRUE(tree.search(10));
    EXPECT_TRUE(tree.search(20));
    EXPECT_TRUE(tree.search(30));
}

TEST(RedBlackTreeTest, RemoveNodeWithOneChild) {
    RedBlackTree tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(5);

    tree.remove(10);
    EXPECT_FALSE(tree.search(10));
    EXPECT_TRUE(tree.search(5));
    EXPECT_TRUE(tree.search(20));
    EXPECT_TRUE(tree.search(30));
}

TEST(RedBlackTreeTest, RemoveNodeWithTwoChildren) {
    RedBlackTree tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    tree.remove(50);
    EXPECT_FALSE(tree.search(50));
    EXPECT_TRUE(tree.search(20));
    EXPECT_TRUE(tree.search(40));
    EXPECT_TRUE(tree.search(60));
    EXPECT_TRUE(tree.search(80));

    tree.remove(30);
    EXPECT_FALSE(tree.search(30));
    EXPECT_TRUE(tree.search(20));
    EXPECT_TRUE(tree.search(40));
}

TEST(RedBlackTreeTest, RemoveAllNodesOneByOne) {
    RedBlackTree tree;
    std::vector<int> values = {40, 20, 60, 10, 30, 50, 70, 5, 15, 25, 35, 45, 55, 65, 75};
    for (int v : values) tree.insert(v);

    for (int v : values) {
        tree.remove(v);
        EXPECT_FALSE(tree.search(v));
    }

    for (int v : values) EXPECT_FALSE(tree.search(v));
}

TEST(RedBlackTreeTest, LargeRandomInsertRemove) {
    RedBlackTree tree;
    std::mt19937 rng(12345);

    std::vector<int> values;
    for (int i = 0; i < 200; ++i) values.push_back(i * 3 + 1);

    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values) tree.insert(v);
    for (int v : values) EXPECT_TRUE(tree.search(v));

    std::shuffle(values.begin(), values.end(), rng);
    for (size_t i = 0; i < values.size() / 2; ++i) {
        SCOPED_TRACE(::testing::Message() << "remove(" << values[i] << ")");
        tree.remove(values[i]);
        EXPECT_FALSE(tree.search(values[i]));
    }
    for (size_t i = values.size() / 2; i < values.size(); ++i) {
        EXPECT_TRUE(tree.search(values[i]));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}