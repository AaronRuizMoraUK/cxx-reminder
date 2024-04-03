#include <cstdio>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>

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

    template<class T>
    void PrintContainerType(const T& container)
    {
        for (const auto& element : container)
        {
            std::printf("%d ", element.getValue());
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
// + Elements continuos in memory, element access is fast
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
    // argumets of the type, not the type itself.
    dynamicArray.push_back(1);

    // Access is fast
    dynamicArray[0] = 3;
    int front2 = staticArrayStd.front();
    int back2 = staticArrayStd.back();
    int at2 = staticArrayStd.at(2);

    // Insert elements at specific position using iterator. Slown.
    // Use emplace to construct and insert.With emplace you pass the constructor
    // argumets of the type, not the type itself.
    std::vector<int>::iterator firstInsertedElementIt = dynamicArray.insert(dynamicArray.begin(), 3);

    // Assign entire vector
    dynamicArray.assign(5, 1); // {1, 1, 1, 1, 1}
    dynamicArray.assign({1, 2, 3, 4, 5});

    std::printf("Dynamic array: ");
    PrintContainer(dynamicArray);
    std::printf("\n\n");

    // Remove elements at the end. Fast.
    dynamicArray.pop_back();

    // Erase elements at specific position using iterator. Slown.
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
// including singly linked lists, doubly linked lists (std::list), and circular linked lists.
// 
// Bad for access and memory, good for modification.
//
// Access: O(1) at the beginning/end, O(n) in the middle
// Search: O(n)
// Insert: O(1) at the beginning/end, O(n) in the middle
// Delete: O(1) at the beginning/end, O(n) in the middle
//
// - Element access is slow (except front and back), as it has to start from the begining of the list.
//      front, back
// - Elements not continuos in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for add/delete at the BEGINNING and END, because it has pointers to front and back elements.
//      push_front, pop_front, push_back, pop_back
// + Good for insertion and deletion of elements once having the iterator. Otherwise a search is needed, which is slow.
//      insert, erase, remove, clear, assign

void LinkedLists()
{
    std::list<int> linkedList = {1, 2, 3, 4, 5};
    std::list<int> linkedList2(5, 1); ; // {1, 1, 1, 1, 1}

    // Add elements at the beginnig and end. Fast.
    linkedList.push_back(5);
    linkedList.push_front(3);

    int front = linkedList.front();
    int back = linkedList.back();

    // Insert elements at specific position using iterator. Fast.
    std::list<int>::iterator firstInsertedElementIt = linkedList.insert(linkedList.begin()++, 2);

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
// - Not possbile to access to any other element, as there are no iterators.
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

// Deques
// 
// A double queue is a queue that can add/delete at both beginning and end.
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
// ~ Elements are stored in multiple chunks of memory, so it's ok to iterate through elements.
//      begin, end
// + Good for add/delete at the BEGINNING and END.
//      push_front, pop_front, push_back, pop_back
// - Bad for insertion and deletion elements in the middle, as elements have to be shifted in memory.
//      insert, erase, clear, assign

void Deques()
{
    std::deque<int> deque = {1, 2, 3, 4};

    // Add elements at the beginnig and end. Fast.
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
// Container where the elements themselves are the key to search for them (instead of an idex)
// If the type used cannot be sorted (operator '<'), an structure with operator '()' can be provided returning less than operation.
// It cannot have repeated elements.
// 
// Underneath it's implemented as a balanced binary search tree, such as red-back tree.
// https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/
// 
// Bad for direct access and memory, good for searches and modifications.
//
// Access: N/A
// Search: O(log n)
// Insert: O(log n)
// Delete: O(log n)
//
// + Element access needs to do a search in the tree, but O(log n) is not terrible.
//      find
// - Elements not continuos in memory, bad for cache when iterating through elements.
//      begin, end (with ++/-- operators only)
// + Good for insertion and deletion of elements
//      insert, erase, extract, clear

void Sets()
{
    std::set<Type, TypeLess> set = {Type(4), Type(3), Type(2), Type(1)};

    // Insert elements. Fast.
    std::pair<std::set<Type, TypeLess>::iterator, bool> insertedElementIt = set.insert(Type(6)); // pair.bool is false as it's new element.
    insertedElementIt = set.insert(Type(6)); // pair.bool is true as it's already inserted.
    insertedElementIt = set.emplace(9);

    std::printf("Set: ");
    PrintContainerType(set);
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    std::set<Type, TypeLess>::iterator afterErasedIt = set.erase(set.begin());
    std::set<Type, TypeLess>::iterator afterErasedRangeIt = set.erase(set.begin(), set.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type.
    auto nodeExtracted = set.extract(set.begin());
    auto nodeExtracted2 = set.extract(Type(9));

    std::set<Type, TypeLess>::iterator findIt = set.find(Type(3));

    set.clear(); // Removes all the elements.
}

// Multisets
// 
// Same as set but it allows to repeate elements.
//
// Access: N/A
// Search: O(log n)
// Insert: O(log n)
// Delete: O(log n)

void Multisets()
{
    std::multiset<Type, TypeLess> set = { Type(4), Type(3), Type(3), Type(2), Type(1), Type(1) };

    // Insert elements. Fast.
    std::multiset<Type, TypeLess>::iterator insertedElementIt = set.insert(Type(6)); 
    insertedElementIt = set.emplace(9);

    std::printf("Multiset: ");
    PrintContainerType(set);
    std::printf("\n\n");

    // Erase elements at specific position using iterator. Fast.
    std::set<Type, TypeLess>::iterator afterErasedIt = set.erase(set.begin());
    std::set<Type, TypeLess>::iterator afterErasedRangeIt = set.erase(set.begin(), set.begin()++); // Removes elements in [first, last) range

    // Remove element from the set and obtain it as node type
    auto nodeExtracted = set.extract(set.begin());
    auto nodeExtracted2 = set.extract(Type(9));

    // If there are several elements with the requested key in the container, any of them may be returned.
    std::set<Type, TypeLess>::iterator findIt = set.find(Type(3));

    set.clear(); // Removes all the elements.
}
