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

    return 0;
}
