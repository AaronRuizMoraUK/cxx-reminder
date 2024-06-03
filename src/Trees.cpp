#include <vector>
#include <stack>
#include <queue>
#include <ranges>

// --------------------------------------------------------------------------------
// Tree
// 
// A tree is recursively defined non-linear (hierarchical) data structure.
// It comprises nodes linked together in a hierarchical manner.
// 
// - Depth: steps from node to root node. Depth of root node is 0.
// - Height: steps from node to its further leaf node. Height of a leaf node is 0.
// - Height of Tree: height of its root node.
// --------------------------------------------------------------------------------

struct Node
{
    Node() = default;
    Node(int data, Node* parent = nullptr)
        : m_nodeData(data)
        , m_parent(parent)
    {
    }
    ~Node()
    {
        std::ranges::for_each(m_children, [](Node* child) { delete child; });
    }

    Node* AddChild(int childData)
    {
        m_children.push_back(new Node(childData, this));
        return m_children.back();
    }

    bool IsRoot() const
    {
        return m_parent == nullptr;
    }

    bool IsLeaf() const
    {
        return m_children.empty();
    }

    int m_nodeData = 0;

    Node* m_parent = nullptr;
    std::vector<Node*> m_children;
};

void TraversePreOrder(const Node* node)
{
    if (!node)
    {
        return;
    }

    std::printf("%d ", node->m_nodeData);

    std::ranges::for_each(node->m_children, [](const Node* child) { TraversePreOrder(child); });
}

void TraverseInOrder(const Node* node)
{
    if (!node)
    {
        return;
    }

    int half = node->m_children.size() / 2;

    std::ranges::for_each(node->m_children | std::views::take(half), [](const Node* child) { TraverseInOrder(child); });

    std::printf("%d ", node->m_nodeData);

    std::ranges::for_each(node->m_children | std::views::drop(half), [](const Node* child) { TraverseInOrder(child); });
}

void TraversePostOrder(const Node* node)
{
    if (!node)
    {
        return;
    }

    std::ranges::for_each(node->m_children, [](const Node* child) { TraversePostOrder(child); });

    std::printf("%d ", node->m_nodeData);
}

void TraverseDepthFirst_NonRecursive(const Node* node)
{
    std::stack<const Node*> stack;
    stack.push(node);

    while (!stack.empty())
    {
        const Node* visit = stack.top();
        stack.pop();

        std::printf("%d ", visit->m_nodeData);

        // Notice the reverse order!
        std::ranges::for_each(visit->m_children | std::views::reverse, [&stack](const Node* child) { stack.push(child); });
    }
}

void TraverseBreathFirst_NonRecursive(const Node* node)
{
    std::queue<const Node*> queue;
    queue.push(node);

    while (!queue.empty())
    {
        const Node* visit = queue.front();
        queue.pop();

        std::printf("%d ", visit->m_nodeData);

        std::ranges::for_each(visit->m_children, [&queue](const Node* child) { queue.push(child); });
    }
}

void Trees()
{
    Node* treeRoot = new Node(1);

    auto* child1 = treeRoot->AddChild(2);
    auto* child2 = treeRoot->AddChild(3);
    auto* child3 = treeRoot->AddChild(4);

    child2->AddChild(5);
    child2->AddChild(6);
    child2->AddChild(7);

    child3->AddChild(8);
    child3->AddChild(9);

    std::printf("TraversePreOrder: ");
    TraversePreOrder(treeRoot); // This is a recursive TraverseDepthFirst
    std::printf("\n");

    std::printf("TraverseInOrder: ");
    TraverseInOrder(treeRoot); // In a Binary Search Tree this traverses in sorted order
    std::printf("\n");

    std::printf("TraversePostOrder: ");
    TraversePostOrder(treeRoot);
    std::printf("\n");

    std::printf("TraverseDepthFirst_NonRecursive: ");
    TraverseDepthFirst_NonRecursive(treeRoot);
    std::printf("\n");

    std::printf("TraverseBreathFirst_NonRecursive: ");
    TraverseBreathFirst_NonRecursive(treeRoot);
    std::printf("\n");

    delete treeRoot;
    std::printf("\n");
}

// --------------------------------------------------------------------------------
// Binary Tree: Tree whose nodes have 2 children (Left and Right)
// 
// - Balanced Tree: if for all nodes it's true that (HeightLeft - HeightRight) <= 1
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Binary Search Tree (BST): Binary tree that is sorted.
// 
// For fast node insertion, removal and search: O(log n)
// --------------------------------------------------------------------------------

struct NodeBST
{
    NodeBST() = default;
    NodeBST(int data, NodeBST* parent = nullptr)
        : m_nodeData(data)
        , m_parent(parent)
    {
    }
    ~NodeBST()
    {
        delete m_left;
        delete m_right;
    }

    // Insert node into tree, it will keep it sorted. O(log n)
    // Returns the new node.
    NodeBST* Insert(int data);

    // Find node into tree. O(log n)
    // Returns null if the node doesn't exist.
    NodeBST* Find(int data);

    // Delete node from tree, it will keep it sorted. O(log n)
    // Returns the parent of the deleted node or new root.
    // Returns null if the node doesn't exist.
    NodeBST* Delete(int data);

    bool IsRoot() const
    {
        return m_parent == nullptr;
    }

    bool IsLeaf() const
    {
        return !m_left && !m_right;
    }

    int m_nodeData = 0;

    NodeBST* m_parent = nullptr;
    NodeBST* m_left = nullptr;
    NodeBST* m_right = nullptr;
};


NodeBST* NodeBST::Insert(int data)
{
    if (data > m_nodeData)
    {
        if (m_right)
        {
            return m_right->Insert(data);
        }
        else
        {
            m_right = new NodeBST(data, this);
            return m_right;
        }
    }
    else
    {
        if (m_left)
        {
            return m_left->Insert(data);
        }
        else
        {
            m_left = new NodeBST(data, this);
            return m_left;
        }
    }
}

NodeBST* NodeBST::Find(int data)
{
    if (data > m_nodeData)
    {
        return (m_right)
            ? m_right->Find(data)
            : nullptr;
    }
    else if (data < m_nodeData)
    {
        return (m_left)
            ? m_left->Find(data)
            : nullptr;
    }
    else
    {
        return this;
    }
}

NodeBST* NodeBST::Delete(int data)
{
    if (data > m_nodeData)
    {
        return (m_right)
            ? m_right->Delete(data)
            : nullptr;
    }
    else if (data < m_nodeData)
    {
        return (m_left)
            ? m_left->Delete(data)
            : nullptr;
    }
    else
    {
        // Case 1: Leaf node. Simple case, delete node and take care of parent's link.
        if (IsLeaf())
        {
            NodeBST* parent = m_parent;
            if (IsRoot())
            {
                delete this;
                return nullptr; // No more nodes in the tree
            }
            else if (this == parent->m_left)
            {
                parent->m_left = nullptr;
                delete this;
                return parent;
            }
            else
            {
                parent->m_right = nullptr;
                delete this;
                return parent;
            }
        }
        // Case 2: One child. Simple case, delete node and connect child to parent.
        else if (m_right == nullptr)
        {
            NodeBST* parent = m_parent;
            NodeBST* child = m_left;
            if (IsRoot())
            {
                delete this;
                return child; // Child is new root
            }
            else if (this == parent->m_left)
            {
                parent->m_left = child;
                delete this;
                return parent;
            }
            else
            {
                parent->m_right = child;
                delete this;
                return parent;
            }
        }
        else if (m_left == nullptr)
        {
            NodeBST* parent = m_parent;
            NodeBST* child = m_right;
            if (IsRoot())
            {
                delete this;
                return child; // Child is new root
            }
            else if (this == parent->m_left)
            {
                parent->m_left = child;
                delete this;
                return parent;
            }
            else
            {
                parent->m_right = child;
                delete this;
                return parent;
            }
        }
        // Case 3: Two children. Replace with max value node under left child, 
        // then delete that node (which will fall under case 1 or 2)
        else
        { 
            NodeBST* maxInLeftChild = m_left;
            while (maxInLeftChild->m_right != nullptr)
            {
                maxInLeftChild = maxInLeftChild->m_right;
            }
            m_nodeData = maxInLeftChild->m_nodeData;
            maxInLeftChild->Delete(m_nodeData);

            if (IsRoot())
            {
                return this; // This node with a new value is now root
            }
            else
            {
                return m_parent;
            }
        }
    }
}

void TraverseInOrder(const NodeBST* node)
{
    if (!node)
    {
        return;
    }

    if (node->m_left)
    {
        TraverseInOrder(node->m_left);
    }

    std::printf("%d ", node->m_nodeData);

    if (node->m_right)
    {
        TraverseInOrder(node->m_right);
    }
}

void TraverseDepthFirst_NonRecursive(const NodeBST* node)
{
    std::stack<const NodeBST*> stack;
    stack.push(node);

    while (!stack.empty())
    {
        const NodeBST* visit = stack.top();
        stack.pop();

        std::printf("%d ", visit->m_nodeData);

        // Notice the reverse order!
        if (visit->m_right)
        {
            stack.push(visit->m_right);
        }
        if (visit->m_left)
        {
            stack.push(visit->m_left);
        }
    }
}

void TraverseBreathFirst_NonRecursive(const NodeBST* node)
{
    std::queue<const NodeBST*> queue;
    queue.push(node);

    while (!queue.empty())
    {
        const NodeBST* visit = queue.front();
        queue.pop();

        std::printf("%d ", visit->m_nodeData);

        if (visit->m_left)
        {
            queue.push(visit->m_left);
        }
        if (visit->m_right)
        {
            queue.push(visit->m_right);
        }
    }
}

void BinarySearchTree()
{
    NodeBST* treeRoot = new NodeBST(30);

    treeRoot->Insert(23);
    treeRoot->Insert(35);

    treeRoot->Insert(11);
    treeRoot->Insert(25);

    treeRoot->Insert(31);
    treeRoot->Insert(42);

    std::printf("BST TraverseDepthFirst_NonRecursive: ");
    TraverseDepthFirst_NonRecursive(treeRoot);
    std::printf("\n");

    std::printf("BST TraverseBreathFirst_NonRecursive: ");
    TraverseBreathFirst_NonRecursive(treeRoot);
    std::printf("\n");

    std::printf("BST TraverseInOrder: ");
    TraverseInOrder(treeRoot); // In a Binary Search Tree this traverses in sorted order
    std::printf("\n");

    treeRoot->Insert(24);

    std::printf("BST Inserted 24: ");
    TraverseDepthFirst_NonRecursive(treeRoot);
    std::printf("\n");

    treeRoot->Delete(24);

    std::printf("BST Deleted 24: ");
    TraverseDepthFirst_NonRecursive(treeRoot);
    std::printf("\n");

    delete treeRoot;
    std::printf("\n");
}

// --------------------------------------------------------------------------------
// Trie. Aka digital tree, radix tree or prefix tree.
// 
// Useful to make dictionaries (words, phone numbers, etc.).
// Nodes do not store their associated key, instead a node's position defines its
// associated key.
// 
// Example: https://en.wikipedia.org/wiki/File:Trie_example.svg
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Quad Tree
// 
// Each node has 4 children. Useful for 2D spatial division.
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Oct Tree
// 
// Each node has 8 children. Useful for 3D spatial division.
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Binary Space Partition (BSP)
// 
// For space partitioning which recursively subdivides an Euclidean space into two
// convex sets by using hyperplanes as partitions.
// 
// Example: https://en.wikipedia.org/wiki/Binary_space_partitioning#Generation
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Heap
// 
// Tree that satisfies the heap property: the children of a node cannot be 
// bigger (max heap) / smaller (min heap) that the parent.
// A heap is NOT a sorted structure overall, it only guarantees the heap property.
// 
// Useful to quickly know the max/min value, as it's always on the root of the tree.
// 
// STL offers std::priority_queue as an implementation of a heap (see DataStructures.cpp)
// --------------------------------------------------------------------------------
