#include <chrono>
#include <functional>
#include <thread>
#include <mutex>
#include <shared_mutex>

namespace
{
    std::string ThreadIdToString(const std::thread::id& id)
    {
        std::ostringstream oss;
        oss << id;
        return oss.str();
    }

    std::string GetNativeThreadId()
    {
        return ThreadIdToString(std::this_thread::get_id());
    }
}

// Threads
//
// Threads allow multiple functions to execute concurrently.

void MainThread(int id, int seconds)
{
    printf("Thread %d) Running for %d seconds (%s)\n", id, seconds, GetNativeThreadId().c_str());
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    printf("Thread %d) Done! (%s)\n", id, GetNativeThreadId().c_str());
}

void Threads()
{
    const int numThreads = 10;

    printf("Main Thread) Creating %d threads... (%s)\n", numThreads, GetNativeThreadId().c_str());

    // Create and run new threads executing 'ThreadMain'
    std::vector<std::unique_ptr<std::thread>> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        auto secondsRange = std::make_pair(2, 5);
        const int waitSeconds = secondsRange.first + std::rand() % (secondsRange.second - secondsRange.first + 1);

        threads.push_back(
            std::make_unique<std::thread>(MainThread, i, waitSeconds));
    }

    printf("Main Thread) Waiting for threads to finish... (%s)\n", GetNativeThreadId().c_str());

    // Wait until threads have finished execution.
    for (auto& thread : threads)
    {
        thread->join();
    }

    printf("Main Thread) Threads finished (%s)\n\n", GetNativeThreadId().c_str());
}

// Mutex and Locks
//
// Used to protect shared data from being simultaneously accessed by multiple threads.

int g_counter = 0;
std::mutex g_mutex;
std::recursive_mutex g_recursiveMutex;

void MainMutex(int id, int increment)
{
    g_mutex.lock();

    printf("Thread %d) Incrementing counter %d by %d = ", id, g_counter, increment);
    g_counter += increment;
    printf("%d\n", g_counter);

    g_mutex.unlock();
}

void MainMutexUsingLock(int id, int increment)
{
    // The mutex is locked/unlocked by the scoped of 'lock' variable.
    // Mutex is locked at construction time, and unlocked when 'lock' is destroys
    // when the function is finished.
    std::lock_guard<std::mutex> lock(g_mutex);

    printf("Thread %d) Incrementing counter %d by %d = ", id, g_counter, increment);
    g_counter += increment;
    printf("%d\n", g_counter);
}

void MainRecursiveMutex(int id, int increment)
{
    // Recursive mutex allows the thread that owns the mutex to lock again.
    std::lock_guard<std::recursive_mutex> lock(g_recursiveMutex);

    if (increment == 0)
    {
        printf("Thread %d) Incremented counter: %d\n", id, g_counter);
    }
    else
    {
        // Increment counter one step at a time recursively
        g_counter++;
        MainRecursiveMutex(id, increment - 1);
    }
}

void MainMutexUsingUniqueLock(int id, int increment)
{
    // Use std::lock_guard when you need a simple, lightweight solution for locking a mutex 
    // for the duration of a scope without the need for manual lock control or deferred locking.
    // Use std::unique_lock when you need more control over mutex locking, such as:
    //   - deferred locking
    //   - manual lock and unlock
    //   - transferring lock ownership.
    std::unique_lock<std::mutex> lock(g_mutex, std::defer_lock);

    lock.lock();

    printf("Thread %d) Incrementing counter %d by %d = ", id, g_counter, increment);
    g_counter += increment;
    printf("%d\n", g_counter);

    lock.unlock(); // Unnecessary as it'd be unlocked in unique_lock destructor.
}

void Mutex()
{
    auto runFunc = [](std::function<void(int, int)> threadMain)
    {
        const int numThreads = 10;
        g_counter = 0;

        printf("Main Thread) Creating %d threads... \n", numThreads);

        // Create and run new threads executing 'ThreadMain'
        std::vector<std::unique_ptr<std::thread>> threads;
        threads.reserve(numThreads);
        for (int i = 0; i < numThreads; ++i)
        {
            threads.push_back(
                std::make_unique<std::thread>(threadMain, i, i + 1));
        }

        printf("Main Thread) Waiting for threads to finish...\n");

        // Wait until threads have finished execution.
        for (auto& thread : threads)
        {
            thread->join();
        }

        printf("Main Thread) Threads finished. Counter: %d\n\n", g_counter);
    };

    runFunc(MainMutex);
    runFunc(MainMutexUsingLock);
    runFunc(MainRecursiveMutex);
    runFunc(MainMutexUsingUniqueLock);
}

// Locking multiple mutex safely

void LockMultipleMutex()
{
    std::mutex mutex1, mutex2, mutex3;

    // RAII helper to check if a mutex is locked or not.
    struct CheckIsLocked
    {
        CheckIsLocked(std::mutex& mutex)
            : m_mutex(mutex)
        {
            m_locked = !m_mutex.try_lock(); // if it's not able to lock, then it's because it's locked already.
        }

        ~CheckIsLocked()
        {
            // if it wasn't locked, that means we locked it with try_lock in constructor, now unlock it.
            if (!m_locked)
            {
                m_mutex.unlock();
            }
        }

        std::mutex& m_mutex;
        bool m_locked = false;
    };

    printf("Mutex locked? 1: %s 2: %s 3: %s\n",
        CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex3).m_locked ? "YES" : "NO");

    // Method 1: Using std::scoped_lock. (C++ 17)
    // 
    // This will take care of waiting until all the mutex provided
    // have been locked, being careful of not causing a deadlock by using
    // a deadlock avoidance algorithm. And at the end of the scope it will
    // unlock all the mutex.
    {
        std::scoped_lock lock(mutex1, mutex2, mutex3);
        // When reaching code here all the mutex have been locked.

        printf("Mutex locked? 1: %s 2: %s 3: %s\n",
            CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
            CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
            CheckIsLocked(mutex3).m_locked ? "YES" : "NO");
    }

    printf("Mutex locked? 1: %s 2: %s 3: %s\n",
        CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex3).m_locked ? "YES" : "NO");

    // Method 2: Using std::lock and std::lock_guard with std::adopt_lock.
    // 
    // Using std::lock will take care of locking all the mutex without
    // causing a deadlock. But std::lock won't unlock the mutex, so std::lock_guard
    // is used so the mutex is unblock at the end of the scope, but since it was already
    // locked it doesn't need to lock it again, that's why std::adopt_lock_t is passed,
    // which means "assume the calling thread already has ownership of the mutex".
    {
        std::lock(mutex1, mutex2, mutex3);
        std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
        std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock);
        std::lock_guard<std::mutex> lock3(mutex3, std::adopt_lock);
        // When reaching code here all the mutex have been locked.

        printf("Mutex locked? 1: %s 2: %s 3: %s\n",
            CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
            CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
            CheckIsLocked(mutex3).m_locked ? "YES" : "NO");
    }

    printf("Mutex locked? 1: %s 2: %s 3: %s\n",
        CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex3).m_locked ? "YES" : "NO");

    // Method 3: Using std::unique_lock with std::adopt_lock and then std::lock.
    // 
    // Similar to method 2, but first using unique_lock with std::defer_lock to
    // indicate "do not acquire ownership of the mutex", then call std::lock to lock
    // the unique_locks (not the mutex like in method 2).
    // But the unique_lock will unclock the mutex when it gets out of the scope.
    // Use this method if you'd like to use the features of unique_lock.
    {
        std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
        std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);
        std::unique_lock<std::mutex> lock3(mutex3, std::defer_lock);
        std::lock(lock1, lock2, lock3);
        // When reaching code here all the mutex have been locked.

        printf("Mutex locked? 1: %s 2: %s 3: %s\n",
            CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
            CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
            CheckIsLocked(mutex3).m_locked ? "YES" : "NO");
    }

    printf("Mutex locked? 1: %s 2: %s 3: %s\n",
        CheckIsLocked(mutex1).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex2).m_locked ? "YES" : "NO",
        CheckIsLocked(mutex3).m_locked ? "YES" : "NO");
}

// Shared Mutex and Share/Unique Locks (C++ 17)
//
// Used to protect shared data from being simultaneously accessed by multiple threads.
// A shared_mutex has two levels of access:
//   - shared: several threads can share ownership of the same mutex.
//   - exclusive: only one thread can own the mutex.
//
// Shared mutex are especially useful when shared data can be safely read by any number
// of threads simultaneously, but a thread may only write the same data when no other thread
// is reading or writing at the same time.

class ThreadSafeCounter
{
public:
    ThreadSafeCounter() = default;

    // Multiple threads/readers can read the counter's value at the same time.
    int Get() const
    {
        std::shared_lock<std::shared_mutex> lock(m_sharedMutex);
        return m_value;
    }

    // Only one thread/writer can increment/write the counter's value.
    void Increment(int threadId)
    {
        std::unique_lock<std::shared_mutex> lock(m_sharedMutex);
        ++m_value;
        printf("Thread %d) Increment Counter %d -> %d\n", threadId, m_value - 1, m_value);
    }

    // Only one thread/writer can reset/write the counter's value.
    void Reset()
    {
        std::unique_lock<std::shared_mutex> lock(m_sharedMutex);
        m_value = 0;
    }

private:
    mutable std::shared_mutex m_sharedMutex; // mutable to be used in get const function
    int m_value = 0;
};

void SharedMutex()
{
    ThreadSafeCounter sharedCounter;

    const int numIncrementsPerThread = 5;

    auto incrementAndPrint = [&sharedCounter, numIncrementsPerThread](int id)
        {
            for (int i = 0; i < numIncrementsPerThread; ++i)
            {
                // Different threads can print this with the same value so long there is
                // no other thread writing.
                printf("Thread %d) Pre-increment Counter %d\n", id, sharedCounter.Get());

                // Only 1 thread can increment at the same time.
                sharedCounter.Increment(id);

                // Different threads can print this with the same value so long there is
                // no other thread writing.
                printf("Thread %d) Post-increment Counter %d\n", id, sharedCounter.Get());
            }
        };

    const int numThreads = 10;

    printf("Main Thread) Creating %d threads... \n", numThreads);

    // Create and run new threads executing 'ThreadMain'
    std::vector<std::unique_ptr<std::thread>> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(
            std::make_unique<std::thread>(incrementAndPrint, i));
    }

    printf("Main Thread) Waiting for threads to finish...\n");

    // Wait until threads have finished execution.
    for (auto& thread : threads)
    {
        thread->join();
    }

    printf("Main Thread) Threads finished. Counter: %d\n\n", sharedCounter.Get());
}

// Atomics
//
// Types that encapsulate values and guarantee atomic operations to prevent data races.
// Useful for lock-free concurrent programming.
// Usual operations are load, store, increment, decrement, add, substract.
// It supports many basic types: https://en.cppreference.com/w/cpp/atomic/atomic

void Atomics()
{
    std::atomic_int atomicCounter(0); // alias of std::atomic<int>

    auto increment = [&atomicCounter](int n)
        {
            for (int i = 0; i < n; ++i)
            {
                atomicCounter++;
            }
        };

    const int numThreads = 10;

    printf("Main Thread) Creating %d threads... \n", numThreads);

    // Create and run new threads executing 'ThreadMain'
    std::vector<std::unique_ptr<std::thread>> threads;
    threads.reserve(numThreads);
    for (int i = 0; i < numThreads; ++i)
    {
        threads.push_back(
            std::make_unique<std::thread>(increment, 333));
    }

    printf("Main Thread) Waiting for threads to finish...\n");

    // Wait until threads have finished execution.
    for (auto& thread : threads)
    {
        thread->join();
    }

    printf("Main Thread) Threads finished. Counter: %d\n\n", atomicCounter.load());
}

// Condition Variables
//
// Used for synchronization between threads. Allows one or more threads to wait for
// notifications sent by other threads.
// It's used with a std::mutex to block one or more threads until another thread
// modifies a shared variable (the condition) and notifies the std::condition_variable.
// std::condition_variable only works with std::unique_lock.

int g_sharedValueForCondition = 0;
std::condition_variable g_conditionVar;
std::mutex g_conditionVarMutex; // This mutex is used for three purposes:
                                // 1) to synchronize accesses g_sharedValueForCondition
                                // 2) to synchronize accesses to printf
                                // 3) for the condition variable g_conditionVar

void MainCVWaits(int id)
{
    // Method 1 using condition variable
    // 1. Acquire a std::unique_lock on the mutex used to protect the shared variable used for the condition.
    // 2. Check the condition
    // 3. Call wait if it the condition is not met yet
    //    NOTE: The wait call migh spuriously wake to check the condition without a notifiation.

    std::unique_lock<std::mutex> lock(g_conditionVarMutex);
    while (g_sharedValueForCondition != 1)
    {
        printf("Wait Thread %d) Waiting...\n", id);
        g_conditionVar.wait(lock);
    }
    printf("Wait Thread %d) Finished waiting. g_sharedValueForCondition == 1\n", id);
}

void MainCVWaitsCompact(int id)
{
    // Method 2 using condition variable
    // 1. Acquire a std::unique_lock on the mutex used to protect the shared variable used for the condition.
    // 2. Call wait passing the function that returns true when the condition is met.
    //    This overload wait encapsulates the check and wait from Method 1.
    //    This overload may be used to ignore spurious awakenings while waiting for a specific condition to become true.

    std::unique_lock<std::mutex> lock(g_conditionVarMutex);
    printf("Wait Thread %d) Waiting...\n", id);
    g_conditionVar.wait(lock, [] { return g_sharedValueForCondition == 1; });
    printf("Wait Thread %d) Finished waiting. g_sharedValueForCondition == 1\n", id);
}

void MainCVSignals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(g_conditionVarMutex);
        printf("Signal Thread) Notifying without changing g_sharedValueForCondition value...\n");
    }
    g_conditionVar.notify_all();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(g_conditionVarMutex);
        g_sharedValueForCondition = 1;
        printf("Signal Thread) Notifying again after changing g_sharedValueForCondition value to 1...\n");
    }
    g_conditionVar.notify_all();
}

void ConditionalVariables()
{
    {
        g_sharedValueForCondition = 0;
        std::thread t1(MainCVWaits, 0), t2(MainCVWaits, 1), t3(MainCVWaits, 2), t4(MainCVSignals);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        printf("\n");
    }

    {
        g_sharedValueForCondition = 0;
        std::thread t1(MainCVWaitsCompact, 0), t2(MainCVWaitsCompact, 1), t3(MainCVWaitsCompact, 2), t4(MainCVSignals);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        printf("\n");
    }
}
