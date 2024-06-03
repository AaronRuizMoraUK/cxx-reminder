#include <cstdio>

void Arrays();
void LinkedLists();
void Stacks();
void Queues();
void PriorityQueues();
void Deques();
void Sets();
void Maps();
void UnorderedSets();
void UnorderedMaps();

void Threads();
void Mutex();
void LockMultipleMutex();
void SharedMutex();
void Atomics();
void ConditionalVariables();
void PromiseAndFuture();

void Reduce();
void TransformReduceWith2Ranges();
void InclusiveScan_ExclusiveScan();
void Find();
void Transform();
void TransformWith2Ranges();
void AdjacentReduce();
void AdjacentInclusiveScan_AdjacentExclusiveScan();
void AdjacentFind();
void AdjacentTransform();
void OtherAlgorithms();

void File();
void FileStreams();
void FileSystem();

void StandardStreamObjects();
void StringStreams();

void Traits();
void TraitsConcepts();

void ThreeWayComparisonOperator();
void Concepts();
void Coroutines();
void Modules();

void Ranges();
void RangeProjections();
void RangeViews();
void RangeViewCompositionAndPipeOperator();
void RangeFactories();

void Trees();
void BinarySearchTree();

int main(int argc, char* argsv[])
{
    std::printf("C++ Reminder\n\n");

    // Data Structures
    Arrays();
    LinkedLists();
    Stacks();
    Queues();
    PriorityQueues();
    Deques();
    Sets();
    Maps();
    UnorderedSets();
    UnorderedMaps();

    // Concurrency
    Threads();
    Mutex();
    LockMultipleMutex();
    SharedMutex();
    Atomics();
    ConditionalVariables();
    PromiseAndFuture();

    // Algorithms
    Reduce(); TransformReduceWith2Ranges();        // Index 1 / Accumulator YES / Operation Reduce
    InclusiveScan_ExclusiveScan();                 // Index 1 / Accumulator YES / Operation Transform
    Find();                                        // Index 1 / Accumulator NO  / Operation Reduce
    Transform(); TransformWith2Ranges();           // Index 1 / Accumulator NO  / Operation Transform
    AdjacentReduce();                              // Index 2 / Accumulator YES / Operation Reduce
    AdjacentInclusiveScan_AdjacentExclusiveScan(); // Index 2 / Accumulator YES / Operation Transform
    AdjacentFind();                                // Index 2 / Accumulator NO  / Operation Reduce
    AdjacentTransform();                           // Index 2 / Accumulator NO  / Operation Transform
    OtherAlgorithms();

    // Files
    File();
    FileStreams();
    FileSystem();

    // Streams
    StandardStreamObjects();
    StringStreams();

    // Traits
    Traits();
    TraitsConcepts();

    // C++20
    ThreeWayComparisonOperator();
    Concepts();
    Coroutines();
    Modules();

    // C++20 Ranges
    Ranges();
    RangeProjections();
    RangeViews();
    RangeViewCompositionAndPipeOperator();
    RangeFactories();

    // Trees
    Trees();
    BinarySearchTree();

    return 0;
}
