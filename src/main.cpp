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
void TransformReduce();
void InclusiveScan_ExclusiveScan();
void Find();
void Transform();
void AdjacentReduce();
void TransformInclusiveScan_TransformExclusiveScan();
void AdjacentFind();
void AdjacentTransform();
void OtherAlgorithms();

void File();
void FileStreams();
void FileSystem();

void StandardStreamObjects();
void StringStreams();

void Traits();

void Concepts();
void Ranges();

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
    Reduce(); TransformReduce();                     // Index 1 / Accumulator YES / Operation Reduce
    InclusiveScan_ExclusiveScan();                   // Index 1 / Accumulator YES / Operation Transform
    Find();                                          // Index 1 / Accumulator NO  / Operation Reduce
    Transform();                                     // Index 1 / Accumulator NO  / Operation Transform
    AdjacentReduce();                                // Index 2 / Accumulator YES / Operation Reduce
    TransformInclusiveScan_TransformExclusiveScan(); // Index 2 / Accumulator YES / Operation Transform
    AdjacentFind();                                  // Index 2 / Accumulator NO  / Operation Reduce
    AdjacentTransform();                             // Index 2 / Accumulator NO  / Operation Transform
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

    // C++20
    Concepts();
    Ranges();

    return 0;
}
