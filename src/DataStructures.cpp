#include <cstdio>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

// Helpers
namespace
{
    template<class T>
    void PrintContainer(const T& container)
    {
        for (const auto& element : container)
        {
            std::printf("%d ", element);
        }
    }

    class Type
    {
    public:
        Type() = default;

        explicit Type(int value)
            : m_value(value)
        {
        }

        int getValue() const
        {
            return m_value;
        }

    private:
        int m_value = 0;
    };

    struct TypeLess
    {
        bool operator()(const Type& left, const Type& right) const
        {
            return left.getValue() < right.getValue();
        }
    };

    struct TypeEqual
    {
        bool operator()(const Type& left, const Type& right) const
        {
            return left.getValue() == right.getValue();
        }
    };

    struct TypeHash
    {
        std::size_t operator()(const Type& element) const
        {
            return std::hash<int>{}(element.getValue());
        }
    };
}

// Arrays
// 
// A collection of elements of the same type placed in contiguous memory locations.
// Arrays can be of fixed size (static arrays) or variable size (dynamic arrays).
// 
// Good for access and memory, bad for modification.
//
// Access: O(1)
// Search: O(n)
// Insert: O(n) (end : O(1) if ignoring the time taken to resize a dynamic array)
// Delete: O(n)
//
// + Elements continuous in memory, element access is fast
//      [], at, front, back
// + Good for cache when iterating through elements.
//      begin, end
// + Good for add/delete at the END (only when there is capacity)
//      push_back, pop_back
// - Bad for insertion and deletion of elements. Needs to allocate new memory and copy elements.
//      insert, erase, clear, assign

void Arrays()
{
    // -----------------
    // Static array
    int staticArray[5] = { 1, 2, 3, 4, 5 };

    std::printf("Static array: ");
    for (int i = 0; i < 5; i++)
    {
        std::printf("%d ", staticArray[i]);
    }
    std::printf("\n\n");

    // -----------------
    // Static array (std)
    std::array<int, 5> staticArrayStd = { 1, 2, 3, 4, 5 };

    // Access is fast
    staticArrayStd[0] = 3;
    int front = staticArrayStd.front();
    int back = staticArrayStd.back();
    int at = staticArrayStd.at(2);

    std::printf("Static array (std): ");
    PrintContainer(staticArrayStd);
    std::printf("\n\n");

    // -----------------
    // Dynamic array
    std::vector<int> dynamicArray;
    std::vector<int> dynamicArray2(5); // Init vector with 5 elements (default value)
    std::vector<int> dynamicArray3(5, 1); // {1, 1, 1, 1, 1}
    std::vector<int> dynamicArray4 = {1, 2, 3, 4, 5};

    dynamicArray.reserve(5); // Reserve enough memory for 5 elements, but size() is still 0.

    // Add elements at the end. Fast if there is capacity.
    // push_back is suitable for inserting elements that already exist,
    // emplace_back provides a more efficient way to construct and insert
    // elements directly into the vector, which can lead to performance
    // improvements in certain scenarios. With emplace_back you pass the constructor
    // arguments of the type, not the type itself.
    dynamicArray.push_back(1);

    // Access is fast
    dynamicArray[0] = 3;
    int front2 = staticArrayStd.front();
    int back2 = staticArrayStd.back();
    int at2 = staticArrayStd.at(2);

    // Insert elements at specific position using iterator. Slow.
    // Use emplace to construct and insert.With emplace you pass the constructor
    // arguments of the type, not the type itself.
    std::vector<int>::iterator firstInsertedElementIt = dynamicArray.insert(dynamicArray.begin(), 3);

    // Assign entire vector
    dynamicArray.assign(5, 1); // {1, 1, 1, 1, 1}
    dynamicArray.assign({1, 2, 3, 4, 5});

    std::printf("Dynamic array: ");
    PrintContainer(dynamicArray);
    std::printf("\n\n");

    // Remove elements at the end. Fast.
    dynamicArray.pop_back();

    // Erase elements at specific position using iterator. Slow.
    std::vector<int>::iterator afterErasedIt = dynamicArray.erase(dynamicArray.begin());
    std::vector<int>::iterator afterErasedRangeIt = dynamicArray.erase(dynamicArray.begin(), dynamicArray.begin() + 2); // Removes elements in [first, last) range

    // Since erase is slow, deleting elements while iterating is very slow O(n2), for example delete all elements with value 5.
    // To improve this the Erase-remove idiom is used (or the std::erase/std::erase_if from C++20)
    //
    // std::remove/std::remove_if does not remove elements from the container, but moves all elements that do not fit the
    // removal criteria to the front of the range, this is done in a single pass through the data range. So all the removed
    // elements are at the tail. It returns the iterator to the first removed element in the tail, so then vector:erase can be used
    // to delete all these elements in one go.
    dynamicArray.erase(
        std::remove(dynamicArray.begin(), dynamicArray.end(), 5), 
        dynamicArray.end()); // Remember to pass end(), otherwise it only deletes the first removed elements.

    std::vector<int>::iterator founddIt = std::find(dynamicArray.begin(), dynamicArray.end(), 5);

    dynamicArray.clear(); // Removes all the elements. Keeps capacity.
}

// Linked Lists
// 
// A collection of nodes that together form a sequence. Each node contains data and
// a pointer to the next node in the sequence. There are several types of linked lists,
// including singly linked lists (std::forward_list), doubly linked lists (std::list)
// and circular linked lists.
// 
// Bad for access and memory, good for modification.
//
// Access: O(1) at the beginning/end, O(n) in the middle
// Search: O(n)
// Insert: O(1) at the beginning/end, O(n) in the middle
// Delete: O(1) at the beginning/end, O(n) in the middle
//
// - Element access is slow (except front and back), as it has to start from the beginning of the list.
//      front, back
// - Elements not continuous in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for add/delete at the BEGINNING and END, because it has pointers to front and back elements.
//      push_front, pop_front, push_back, pop_back
// + Good for insertion and deletion of elements once having the iterator. Otherwise a search is needed, which is slow.
//      insert, erase, splice, remove, clear, assign

void LinkedLists()
{
    // Doubly Linked List (std::list)
    std::list<int> linkedList = {1, 2, 3, 4, 5};
    std::list<int> linkedList2(5, 1); ; // {1, 1, 1, 1, 1}

    // Add elements at the beginning and end. Fast.
    linkedList.push_back(5);
    linkedList.push_front(3);

    int front = linkedList.front();
    int back = linkedList.back();

    // Insert elements at specific position using iterator. Fast.
    std::list<int>::iterator firstInsertedElementIt = linkedList.insert(linkedList.begin()++, 2);

    // Splice moves elements from one list to another into a specific location, same as insert but moving.
    linkedList.splice(linkedList.begin()++, linkedList2);

    // Assign entire list
    linkedList.assign(5, 1); // {1, 1, 1, 1, 1}
    linkedList.assign({ 1, 2, 3, 4, 5 });

    std::printf("Linked List: ");
    PrintContainer(linkedList);
    std::printf("\n\n");

    // Remove elements at the begnning and end. Fast.
    linkedList.pop_back();
    linkedList.pop_front();

    // Erase elements at specific position using iterator. Fast.
    std::list<int>::iterator afterErasedIt = linkedList.erase(linkedList.begin());
    std::list<int>::iterator afterErasedRangeIt = linkedList.erase(linkedList.begin(), linkedList.begin()++); // Removes elements in [first, last) range

    // Remove elements. Slow as it needs to search it.
    linkedList.remove(4); // Remove all elements of value 4.
    linkedList.remove_if([](int element) { return element > 4; }); // Remove all elements larger than 4.

    std::list<int>::iterator founddIt = std::find(linkedList.begin(), linkedList.end(), 3);

    linkedList.clear(); // Removes all the elements.

    // ---------------------------
    // Singly Linked List (std::forward_list)
    // Same as std::list but it can only travel forward and add/delete at front (not back) of the list.
    // It uses less memory per element as it only has 1 pointer to the next element.
    //
    // Access: O(1) at the beginning, O(n) in the middle/end
    // Search: O(n)
    // Insert: O(1) at the beginning, O(n) in the middle/end
    // Delete: O(1) at the beginning, O(n) in the middle/end

    std::forward_list<int> forwardLinkedList = { 1, 2, 3, 4, 5 };
    std::forward_list<int> forwardLinkedList2(5, 1); ; // {1, 1, 1, 1, 1}

    // Add elements at the beginnig. Fast.
    forwardLinkedList.push_front(3);

    int front2 = forwardLinkedList.front();

    // Insert elements at specific position using iterator. Fast.
    std::forward_list<int>::iterator firstInsertedElementIt2 = forwardLinkedList.insert_after(forwardLinkedList.begin(), 2);

    // Splice moves elements from one list to another into a specific location, same as insert but moving.
    forwardLinkedList.splice_after(forwardLinkedList.begin(), forwardLinkedList2);

    // Assign entire list
    forwardLinkedList.assign(5, 1); // {1, 1, 1, 1, 1}
    forwardLinkedList.assign({ 1, 2, 3, 4, 5 });

    std::printf("Forward Linked List: ");
    PrintContainer(forwardLinkedList);
    std::printf("\n\n");

    // Remove elements at the beginning. Fast.
    forwardLinkedList.pop_front();

    // Erase elements at specific position using iterator. Fast.
    std::forward_list<int>::iterator afterErasedIt2 = forwardLinkedList.erase_after(forwardLinkedList.before_begin()); // Removes first element
    afterErasedIt2 = forwardLinkedList.erase_after(forwardLinkedList.begin()); // Removes element AFTER the iterator
    std::forward_list<int>::iterator afterErasedRangeIt2 = forwardLinkedList.erase_after(forwardLinkedList.before_begin(), forwardLinkedList.begin()++); // Removes elements in (first, last) range

    // Remove elements. Slow as it needs to search it.
    forwardLinkedList.remove(4); // Remove all elements of value 4.
    forwardLinkedList.remove_if([](int element) { return element > 4; }); // Remove all elements larger than 4.

    std::forward_list<int>::iterator founddIt2 = std::find(forwardLinkedList.begin(), forwardLinkedList.end(), 3);

    forwardLinkedList.clear(); // Removes all the elements.
}

// Stacks
// 
// A linear data structure that follows the Last In First Out (LIFO) principle.
// Operations are mainly push (add an element) and pop (remove an element).
// Stacks can be implemented using arrays or linked lists.
// 
// std::stack underneath is an adapter of std::deque.
// 
// Good for access and modify only the top element, not possible to access/modify any other elements.
//
// Access(top): O(1)
// Search: N/A
// Insert(push): O(1)
// Delete(pop): O(1)
//
// + Access to top element is fast
//      top
// - Not possible to access to any other element, as there are no iterators.
// + Good for add/delete at the top.
//      push, pop
// - Not possible for insertion and deletion other than the top.

void Stacks()
{
    std::stack<int> stack;

    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);

    int top = stack.top();

    std::printf("Stack: ");
    while (!stack.empty()) {
        std::printf("%d ", stack.top());
        stack.pop();
    }
    std::printf("\n\n");
}

// Queues
// 
// A linear data structure that follows the First In First Out (FIFO) principle.
// Operations are mainly enqueue (add an element) and dequeue (remove an element).
// Queues can be implemented using arrays or linked lists.
// 
// std::queue underneath is an adapter of std::deque.
// 
// Good for access front and back elements, not possible to access/modify any other elements.
//
// Access(front/back): O(1)
// Search: N/A
// Insert(push): O(1)
// Delete(pop): O(1)
//
// + Access to front and back elements is fast
//      front, back
// - Not possible to access to any other element, as there are no iterators.
// + Good for add at the end and remove from start.
//      push, pop
// - Not possible for insertion and deletion other than add at the end and remove from start.

void Queues()
{
    std::queue<int> queue;

    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);

    int front = queue.front();
    int back = queue.back();

    std::printf("Queue: ");
    while (!queue.empty()) {
        std::printf("%d ", queue.front());
        queue.pop();
    }
    std::printf("\n\n");
}

// Priority Queues
// 
// A linear data structure that provides constant time lookup of the largest (or smallest, 
// depending on the comparator) element, at the expense of logarithmic insertion and extraction. 
// It is typically implemented using a heap data structure, such as a binary heap.
// 
// Good for access to top element (largest or smallest), not possible to access/modify any other elements.
//
// Access(top element): O(1)
// Search: N/A
// Insert(push): O(log n)
// Delete(pop top): O(log n)
//
// + Access to top element (largest or smallest) is fast
//      top
// - Not possible to access to any other element, as there are no iterators.
// + Good for add new elements and removing top element.
//      push, pop
// - Not possible for deletion except from top.

void PriorityQueues()
{
    // Max priority queue
    std::priority_queue<int> maxQueue;

    maxQueue.push(1);
    maxQueue.push(2);
    maxQueue.push(3);
    maxQueue.push(4);

    int max = maxQueue.top();

    std::printf("Priority Queue (max): ");
    while (!maxQueue.empty()) {
        std::printf("%d ", maxQueue.top());
        maxQueue.pop();
    }
    std::printf("\n\n");

    // -------------------
    // Min priority queue
    std::priority_queue<int, std::vector<int>, std::greater<int>> minQueue;

    minQueue.push(1);
    minQueue.push(2);
    minQueue.push(3);
    minQueue.push(4);

    int min = minQueue.top();

    std::printf("Priority Queue (min): ");
    while (!minQueue.empty()) {
        std::printf("%d ", minQueue.top());
        minQueue.pop();
    }
    std::printf("\n\n");
}

// Deques
// 
// A double-ended queue is a queue that can add/delete at both beginning and end.
// It also has iterators for access to any element.
// 
// https://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl
// 
// Good for access. Good to add/delete at the beginning and the end. Bad for add/delete in the middle.
//
// Access: O(1)
// Search: O(N)
// Insert: O(1) at the beginning/end, O(n) in the middle
// Delete: O(1) at the beginning/end, O(n) in the middle
//
// + Element access is fast
//      [], at, front, back
// ~ Elements are stored in multiple chunks of memory, so it's OK to iterate through elements.
//      begin, end
// + Good for add/delete at the BEGINNING and END.
//      push_front, pop_front, push_back, pop_back
// - Bad for insertion and deletion elements in the middle, as elements have to be shifted in memory.
//      insert, erase, clear, assign

void Deques()
{
    std::deque<int> deque = {1, 2, 3, 4};

    // Add elements at the beginning and end. Fast.
    deque.push_back(5);
    deque.push_front(2);

    // Access is fast
    deque[0] = 3;
    int front = deque.front();
    int back = deque.back();
    int at = deque.at(2);

    // Insert elements at specific position using iterator. Slow.
    std::deque<int>::iterator firstInsertedElementIt = deque.insert(deque.begin() + 2, 3);

    // Assign entire deque
    deque.assign(5, 1); // {1, 1, 1, 1, 1}
    deque.assign({ 1, 2, 3, 4, 5 });

    std::printf("Deque: ");
    PrintContainer(deque);
    std::printf("\n\n");

    // Remove elements at the beginning and the end. Fast.
    deque.pop_back();
    deque.pop_front();

    // Erase elements at specific position using iterator. Slow.
    std::deque<int>::iterator afterErasedIt = deque.erase(deque.begin());
    std::deque<int>::iterator afterErasedRangeIt = deque.erase(deque.begin(), deque.begin() + 2); // Removes elements in [first, last) range

    std::deque<int>::iterator founddIt = std::find(deque.begin(), deque.end(), 3);

    deque.clear(); // Removes all the elements.
}

// Sets
// 
// Container where the elements themselves are the key to search for them (instead of an index), and they store with a sorted order.
// If the type used cannot be sorted (operator '<'), an structure with operator '()' can be provided returning less than operation.
// It cannot have repeated elements.
// It cannot modify elements (as that would modify its sorted order and corrupt the container).
// 
// Underneath it's implemented as a balanced binary search tree, such as red-back tree.
// https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
// 
// Good for searches and modifications. Bad that accesses are searches.
//
// Access: N/A
// Search: O(log n)
// Insert: O(log n)
// Delete: O(log n)
//
// + Element access needs to do a search in the tree, but O(log n) is not terrible.
//      find, equal_range, lower_bound, upper_bound
// - Elements not continuous in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for insertion and deletion of elements
//      insert, erase, merge, extract, clear

void Sets()
{
    std::set<Type, TypeLess> set = {Type(4), Type(3), Type(2), Type(1)};

    // Insert elements. Fast.
    std::pair<std::set<Type, TypeLess>::iterator, bool> insertedElementPair = set.insert(Type(6)); // pair.bool is false as it's new element.
    insertedElementPair = set.insert(Type(6)); // pair.bool is true as it's already inserted.
    insertedElementPair = set.emplace(9);

    // Search is O(log n), which is good.
    std::set<Type, TypeLess>::iterator findIt = set.find(Type(3));

    std::printf("Set: ");
    for (const auto& element : set)
    {
        std::printf("%d ", element.getValue());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    std::set<Type, TypeLess>::iterator afterErasedIt = set.erase(set.begin());
    std::set<Type, TypeLess>::iterator afterErasedRangeIt = set.erase(set.begin(), set.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted = set.extract(set.begin());
    auto nodeExtracted2 = set.extract(Type(9));

    // Merge splices (moves elements) from another set.
    // If element already exists it doesn't move it.
    std::set<Type, TypeLess> set2 = { Type(3), Type(52), Type(105) };
    set.merge(set2);

    set.clear(); // Removes all the elements.

    // ---------------------------
    // Multisets
    // Same as set but it allows to repeat elements.

    std::multiset<Type, TypeLess> multiset = { Type(4), Type(3), Type(3), Type(2), Type(1), Type(1) };

    // Insert elements. Fast.
    std::multiset<Type, TypeLess>::iterator insertedElementIt = multiset.insert(Type(6));
    insertedElementIt = multiset.emplace(9);

    // Search is O(log n), which is good.
    // If there are several elements with the requested key in the container, any of them may be returned.
    std::multiset<Type, TypeLess>::iterator findIt2 = multiset.find(Type(3));

    std::printf("Multiset: ");
    for (const auto& element : multiset)
    {
        std::printf("%d ", element.getValue());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    std::multiset<Type, TypeLess>::iterator afterErasedIt2 = multiset.erase(multiset.begin());
    std::multiset<Type, TypeLess>::iterator afterErasedRangeIt2 = multiset.erase(multiset.begin(), multiset.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type
    auto nodeExtracted3 = multiset.extract(multiset.begin());
    auto nodeExtracted4 = multiset.extract(Type(9));

    // Merge splices (moves elements) from another multiset, including repeated elements.
    std::multiset<Type, TypeLess> multiset2 = { Type(3), Type(52), Type(105) };
    multiset.merge(multiset2);

    multiset.clear(); // Removes all the elements.
}

// Maps
// 
// Container that stores key-value pairs, with each key being unique, and the keys are stored in a sorted order.
// If the key type used cannot be sorted (operator '<'), an structure with operator '()' can be provided returning less than operation.
// It cannot have repeated keys.
// 
// Underneath it's implemented as a balanced binary search tree, such as red-back tree.
// https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
// 
// Good for searches and modifications. Bad that accesses are searches.
//
// Access: N/A
// Search: O(log n)
// Insert: O(log n)
// Delete: O(log n)
//
// + Element access needs to do a search in the tree, but O(log n) is not terrible.
//      [], at, find, equal_range, lower_bound, upper_bound
// - Elements not continuous in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for insertion and deletion of elements
//      insert, insert_or_assign, erase, merge, extract, clear

void Maps()
{
    using MyMap = std::map<Type, std::string, TypeLess>;
    MyMap map = 
    {
        {Type(4), "four"}, 
        {Type(3), "three"}, 
        {Type(2), "two"},
        {Type(1), "one"}
    };

    // Insert elements. Fast.
    std::pair<MyMap::iterator, bool> insertedElementPair = map.insert({ Type(6), "six" }); // pair.bool is false as it's new element.
    insertedElementPair = map.insert({ Type(6), "six_2" }); // pair.bool is true as Type(6) key is already inserted, it didn't assign "six_2" value.

    insertedElementPair = map.emplace(Type(6), "six_3"); // Same as insert with in-place construction
    insertedElementPair = map.try_emplace(Type(6), "six_4"); // Same as emplace, but it doesn't move from rvalue arguments if the insertion does not happen.

    insertedElementPair = map.insert_or_assign(Type(6), "six_override"); // Same as insert, but it does assign the new value if key is found.

    // Search is O(log n), which is good.
    map[Type(9)] = "nine"; // [] operator does an insertion of default value (empty string) if key does not exist. Then it gets assigned "nine".
    std::string value = map.at(Type(6)); // If key is not found, an exception of type std::out_of_range is thrown. Better use find instead.
    MyMap::iterator findIt = map.find(Type(6));

    std::printf("Map: ");
    for (const auto& pair : map)
    {
        std::printf("{%d, \"%s\"} ", pair.first.getValue(), pair.second.c_str());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    MyMap::iterator afterErasedIt = map.erase(map.begin());
    MyMap::iterator afterErasedRangeIt = map.erase(map.begin(), map.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted = map.extract(map.begin());
    auto nodeExtracted2 = map.extract(Type(9));

    // Merge splices (moves elements) from another map.
    // If key already exists it doesn't move it.
    MyMap map2 =
    {
        {Type(4), "four"},
        {Type(33), "thirty three"},
        {Type(22), "twenty two"}
    };
    map.merge(map2);

    map.clear(); // Removes all the elements.

    // ---------------------------
    // Multimaps
    // Same as maps but it allows to repeat keys.

    using MyMultimap = std::multimap<Type, std::string, TypeLess>;
    MyMultimap multimap =
    {
        {Type(4), "four"},
        {Type(3), "three"},
        {Type(3), "three_2"},
        {Type(2), "two"},
        {Type(2), "two_2"},
        {Type(1), "one"}
    };

    // Insert elements. Fast.
    MyMultimap::iterator insertedElemenIt = multimap.insert({ Type(6), "six" });
    insertedElemenIt = multimap.insert({ Type(6), "six_2" });
    insertedElemenIt = multimap.emplace(Type(6), "six_3");

    // Search is O(log n), which is good.
    // If there are several elements with the requested key in the container, any of them may be returned.
    MyMultimap::iterator findIt2 = multimap.find(Type(6));

    std::printf("Multimap: ");
    for (const auto& pair : multimap)
    {
        std::printf("{%d, \"%s\"} ", pair.first.getValue(), pair.second.c_str());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    MyMultimap::iterator afterErasedIt2 = multimap.erase(multimap.begin());
    MyMultimap::iterator afterErasedRangeIt2 = multimap.erase(multimap.begin(), multimap.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted3 = multimap.extract(multimap.begin());
    auto nodeExtracted4 = multimap.extract(Type(9));

    // Merge splices (moves elements) from another multimap, including repeated keys.
    MyMultimap multimap2 =
    {
        {Type(4), "four"},
        {Type(33), "thirty three"},
        {Type(33), "thirty three 2"},
        {Type(22), "twenty two"}
    };
    multimap.merge(multimap2);

    multimap.clear(); // Removes all the elements.
}

// Unordered Sets
// 
// Same as std::set but using a hash table instead of a tree.
// 
// Container where the elements themselves are the key to search for them (instead of an index).
// The elements are not sorted in any particular order, but organized into buckets.
// Which bucket an element is placed into depends entirely on the hash of its value.
// This allows fast access to individual elements, since once a hash is computed, it refers to the exact bucket the element is placed into.
// If the type used cannot be hashed, an structure with operator '()' can be provided returning the hash calculation.
// If the type used cannot be compared (operator '=='), an structure with operator '()' can be provided returning equal than operation.
// It cannot have repeated elements.
// It cannot modify elements' values involved in the hashing (as that would modify its sorted order and corrupt the container).
// 
// The performance heavily depends on the quality of the hash function used. A good hash function distributes elements uniformly across buckets, 
// minimizing collisions and maintaining average-case time complexity.
// Hash tables have the concept of load factor "n / b", where n is the number of elements and b is the number of buckets.
// If load factor is low: waste of space as there are too many buckets for the number of elements (they're not going to collide though)
// If load factor is high: Risk of hash collisions and buckets may contain many different elements, making operations less efficient.
// 
// Good for searches and modifications. Bad that accesses are searches, buckets takes memory and hash calculation might be computationally expensive.
//
// Access: N/A
// Search: O(1)
// Insert: O(1)
// Delete: O(1)
//
// + Element access is direct and fast (when using a good hash function).
//      find, equal_range
// - Elements not continuous in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for insertion and deletion of elements
//      insert, erase, merge, extract, clear

void UnorderedSets()
{
    using MyUnorderedSet = std::unordered_set<Type, TypeHash, TypeEqual>;
    MyUnorderedSet set = { Type(4), Type(3), Type(2), Type(1) };

    // Insert elements. Fast.
    std::pair<MyUnorderedSet::iterator, bool> insertedElementPair = set.insert(Type(6)); // pair.bool is false as it's new element.
    insertedElementPair = set.insert(Type(6)); // pair.bool is true as it's already inserted.
    insertedElementPair = set.emplace(9);

    // Search is O(log n), which is good.
    MyUnorderedSet::iterator findIt = set.find(Type(3));

    std::printf("Unordered Set: ");
    for (const auto& element : set)
    {
        std::printf("%d ", element.getValue());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    MyUnorderedSet::iterator afterErasedIt = set.erase(set.begin());
    MyUnorderedSet::iterator afterErasedRangeIt = set.erase(set.begin(), set.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted = set.extract(set.begin());
    auto nodeExtracted2 = set.extract(Type(9));

    // Merge splices (moves elements) from another set.
    // If element already exists it doesn't move it.
    MyUnorderedSet set2 = { Type(3), Type(52), Type(105) };
    set.merge(set2);

    set.clear(); // Removes all the elements.

    // ---------------------------
    // Unordered Multisets
    // Same as unordered_set but it allows to repeat elements.
    using MyUnorderedMultiset = std::unordered_multiset<Type, TypeHash, TypeEqual>;
    MyUnorderedMultiset multiset = { Type(4), Type(3), Type(3), Type(2), Type(1), Type(1) };

    // Insert elements. Fast.
    MyUnorderedMultiset::iterator insertedElementIt = multiset.insert(Type(6));
    insertedElementIt = multiset.emplace(9);

    // Search is O(log n), which is good.
    // If there are several elements with the requested key in the container, any of them may be returned.
    MyUnorderedMultiset::iterator findIt2 = multiset.find(Type(3));

    std::printf("Unordered Multiset: ");
    for (const auto& element : multiset)
    {
        std::printf("%d ", element.getValue());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    MyUnorderedMultiset::iterator afterErasedIt2 = multiset.erase(multiset.begin());
    MyUnorderedMultiset::iterator afterErasedRangeIt2 = multiset.erase(multiset.begin(), multiset.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type
    auto nodeExtracted3 = multiset.extract(multiset.begin());
    auto nodeExtracted4 = multiset.extract(Type(9));

    // Merge splices (moves elements) from another multiset, including repeated elements.
    MyUnorderedMultiset multiset2 = { Type(3), Type(52), Type(105) };
    multiset.merge(multiset2);

    multiset.clear(); // Removes all the elements.
}

// Unordered Maps
// 
// Same as std::map but using a hash table instead of a tree.
// 
// Container that stores key-value pairs, with each key being unique.
// The elements are not sorted in any particular order, but organized into buckets.
// Which bucket an element is placed into depends entirely on the hash of its key.
// This allows fast access to individual elements, since once a key hash is computed, it refers to the exact bucket the element is placed into.
// If the key type used cannot be hashed, an structure with operator '()' can be provided returning the hash calculation.
// If the key type used cannot be compared (operator '=='), an structure with operator '()' can be provided returning equal than operation.
// It cannot have repeated keys.
// 
// The performance heavily depends on the quality of the hash function used. A good hash function distributes elements uniformly across buckets, 
// minimizing collisions and maintaining average-case time complexity.
// Hash tables have the concept of load factor "n / b", where n is the number of elements and b is the number of buckets.
// If load factor is low: waste of space as there are too many buckets for the number of elements (they're not going to collide though)
// If load factor is high: Risk of hash collisions and buckets may contain many different elements, making operations less efficient.
// 
// Good for searches and modifications. Bad that accesses are searches, buckets takes memory and hash calculation might be computationally expensive.
//
// Access: N/A
// Search: O(1)
// Insert: O(1)
// Delete: O(1)
//
// + Element access is direct and fast (when using a good hash function).
//      [], at, find, equal_range
// - Elements not continuous in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for insertion and deletion of elements
//      insert, insert_or_assign, erase, merge, extract, clear

void UnorderedMaps()
{
    using MyUnordereMap = std::unordered_map<Type, std::string, TypeHash, TypeEqual>;
    MyUnordereMap map =
    {
        {Type(4), "four"},
        {Type(3), "three"},
        {Type(2), "two"},
        {Type(1), "one"}
    };

    // Insert elements. Fast.
    std::pair<MyUnordereMap::iterator, bool> insertedElementPair = map.insert({ Type(6), "six" }); // pair.bool is false as it's new element.
    insertedElementPair = map.insert({ Type(6), "six_2" }); // pair.bool is true as Type(6) key is already inserted, it didn't assign "six_2" value.

    insertedElementPair = map.emplace(Type(6), "six_3"); // Same as insert with in-place construction
    insertedElementPair = map.try_emplace(Type(6), "six_4"); // Same as emplace, but it doesn't move from rvalue arguments if the insertion does not happen.

    insertedElementPair = map.insert_or_assign(Type(6), "six_override"); // Same as insert, but it does assign the new value if key is found.

    // Search is O(log n), which is good.
    map[Type(9)] = "nine"; // [] operator does an insertion of default value (empty string) if key does not exist. Then it gets assigned "nine".
    std::string value = map.at(Type(6)); // If key is not found, an exception of type std::out_of_range is thrown. Better use find instead.
    MyUnordereMap::iterator findIt = map.find(Type(6));

    std::printf("Unordered Map: ");
    for (const auto& pair : map)
    {
        std::printf("{%d, \"%s\"} ", pair.first.getValue(), pair.second.c_str());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    MyUnordereMap::iterator afterErasedIt = map.erase(map.begin());
    MyUnordereMap::iterator afterErasedRangeIt = map.erase(map.begin(), map.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted = map.extract(map.begin());
    auto nodeExtracted2 = map.extract(Type(9));

    // Merge splices (moves elements) from another map.
    // If key already exists it doesn't move it.
    MyUnordereMap map2 =
    {
        {Type(4), "four"},
        {Type(33), "thirty three"},
        {Type(22), "twenty two"}
    };
    map.merge(map2);

    map.clear(); // Removes all the elements.

    // ---------------------------
    // Unordered Multimaps
    // Same as unordered_map but it allows to repeat keys.

    using MyUnorderedMultimap = std::unordered_multimap<Type, std::string, TypeHash, TypeEqual>;
    MyUnorderedMultimap multimap =
    {
        {Type(4), "four"},
        {Type(3), "three"},
        {Type(3), "three_2"},
        {Type(2), "two"},
        {Type(2), "two_2"},
        {Type(1), "one"}
    };

    // Insert elements. Fast.
    MyUnorderedMultimap::iterator insertedElemenIt = multimap.insert({ Type(6), "six" });
    insertedElemenIt = multimap.insert({ Type(6), "six_2" });
    insertedElemenIt = multimap.emplace(Type(6), "six_3");

    // Search is O(log n), which is good.
    // If there are several elements with the requested key in the container, any of them may be returned.
    MyUnorderedMultimap::iterator findIt2 = multimap.find(Type(6));

    std::printf("Unordered Multimap: ");
    for (const auto& pair : multimap)
    {
        std::printf("{%d, \"%s\"} ", pair.first.getValue(), pair.second.c_str());
    }
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    MyUnorderedMultimap::iterator afterErasedIt2 = multimap.erase(multimap.begin());
    MyUnorderedMultimap::iterator afterErasedRangeIt2 = multimap.erase(multimap.begin(), multimap.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted3 = multimap.extract(multimap.begin());
    auto nodeExtracted4 = multimap.extract(Type(9));

    // Merge splices (moves elements) from another multimap, including repeated keys.
    MyUnorderedMultimap multimap2 =
    {
        {Type(4), "four"},
        {Type(33), "thirty three"},
        {Type(33), "thirty three 2"},
        {Type(22), "twenty two"}
    };
    multimap.merge(multimap2);

    multimap.clear(); // Removes all the elements.
}
