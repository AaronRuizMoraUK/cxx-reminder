#include <iostream>
#include <cstdio>

// --------------------------------------------------------------------------------
// Link to C++20 new features cheat-sheet
//
// https://github.com/AaronRuizMoraUK/modern-cpp-features/blob/master/CPP20.md
//
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Concepts
// 
// Mechanism to place constraints on your template type parameters
// For example, constrain that only integers can use our templated function.
// If the constraint is not satisfied, it will produce a compiler error.
// There are standard built in concepts, but you can build your own concepts.
// 
// Concepts are a much cleaner way than using template traits and static_asserts.
// 
// https://en.cppreference.com/w/cpp/concepts
// --------------------------------------------------------------------------------

#include <concepts>

template<typename T>
    requires std::integral<T> // <-- Boolean expression evaluated in compilation time. Traits also work here.
T Add(T a, T b)
{
    return a + b;
}

// Another syntax for using concepts
template<std::floating_point T>
T Add2(T a, T b)
{
    return a + b;
}

// Another syntax using auto, no need to specify template.
std::signed_integral auto Add3(std::signed_integral auto a, std::signed_integral auto b)
{
    return a + b;
}

// Yet another valid syntax for using concepts
template<typename T>
T Add4(T a, T b)
    requires (std::integral<T> || std::floating_point<T>)
{
    return a + b;
}

template<typename T>
T Add5(T a, T b)
    requires std::convertible_to<T, double>
{
    return a + b;
}

// Defining a concept
template<typename T>
concept MyIntegral = std::integral<T>; // <-- Boolean expression evaluated in compilation time. Traits also work here.

// Define a concept to check the type passed support operators
template <typename T>
concept Multipliable = requires(T a, T b)
{
    a * b; // Just makes sure the syntax is valid, otherwise it's compilation error, for example if std::string were used.
};

template <typename T>
concept Incrementable = requires(T a)
{
    // Type T must support all these operations, the syntax must be correct.
    a += 1;
    ++a;
    a++;
};

template<typename T>
    requires MyIntegral<T> && Multipliable<T> && Incrementable<T>
T Add6(T a, T b)
{
    return a + b;
}

template <typename T>
concept TinyType = requires(T a)
{
    // MISLEADING!! This only checks syntax, it does NOT evaluate the expression!
    // So using double passes this requirement, because double can also do sizeof and 
    // compare its result.
    // To evaluate the expression, use nested requirements, see TinyType2 below.
    sizeof(T) <= 4;
};

template <typename T>
concept TinyType2 = requires(T a)
{
    // Using Nested Requirements it does evaluate the expression.
    // Now using double will NOT pass the requirement.
    requires sizeof(T) <= 4;
};

template<typename T>
concept Addable = requires(T a, T b)
{
    // This is called Compound Requirement. The result value after + operator must be convertible to int.
    // It doesn't mean that it will convert the result int, it just means that the type used should be able
    // to be converted to int.
    { a + b } -> std::convertible_to<int>;
};

template <typename T>
concept HasValueMember = requires {
    // This is called Type Requirements on type `T`
    typename T::value; // It must have an inner member named `value`
};

void Concepts()
{
    int a = 5;
    int b = 3;
    float af = 5.0f;
    float bf = 3.0f;
    auto result = Add(a, b);
    auto result2 = Add2(af, bf);
    auto result3 = Add3(a, b);
    auto result4 = Add4(af, bf);
    auto result5 = Add5(af, bf);
    auto result6 = Add6(a, b);

    // Variables can also be declared using concepts (using auto) and
    // it won't compile if the type won't pass its constrains.
    std::integral auto x = Add(10, 20);
}

// --------------------------------------------------------------------------------
// Coroutines
// --------------------------------------------------------------------------------

#include <coroutine>

// Coroutines are basically functions that can be paused and resumed.
// Coroutines are designed to make writing asynchronous code easier.
// When a coroutine is paused, its state (variables, parameters, execution point, etc.)
// is saved in heap memory. Then with the use of a handle it can be resumed later.
// 
// Note that coroutines do not run concurrently, they still use the stack in such way that:
// - When paused, the coroutine is removed from stack and its state is stored in heap memory.
// - When resumed, the coroutine is added to the stack and its execution is resumed from where it was left off.
//
// C++20 new keywords to pause and resume coroutines:
// - co_await: suspends execution until resumed
// - co_yield:  suspends execution until resumed returning a value
// - co_return: completes execution and optionally returns a value
//
// If a function uses any of these keywords, it becomes a coroutine. There is no
// other special syntax for coroutines.
//
// But there are no predefined coroutines types in C++20, you have to create them following
// the requirements using the coroutine infrastructure. A coroutine type basically defines
// how the coroutine works, how it suspends, resumes, etc. And it holds the coroutine handle.
// 
// It's expected that C++23 will provide high level coroutine types built into C++.

// -------------------------
// Let's create a simple coroutine type using coroutine infrastructure.

template<typename T>
struct Promise;

// Coroutine type with the minimum requirements, managing the coroutine handle.
struct CoroutineType
{
    // It's required to declare a type named exactly 'promise_type'.
    // The promise type will define how this coroutine works.
    using promise_type = Promise<int>;

    using CoroutineHandle = std::coroutine_handle<promise_type>;

    CoroutineType(promise_type& promiseType)
        : m_handle(CoroutineHandle::from_promise(promiseType))
    {
    }

    ~CoroutineType()
    {
        m_handle.destroy();
        printf("Handle destroyed!\n");
    }

    CoroutineHandle m_handle;
};

// Promise type providing simple coroutine functionality with support for 'co_yield value;' and 'co_return value;'
// 
// NOTE: The member functions are required with the exact naming.
template<typename T>
struct Promise
{
    // Creates the coroutine type object
    CoroutineType get_return_object()
    {
        return CoroutineType(*this);
    }

    // Default action when coroutine is called.
    // std::suspend_always is an awaitable object. An awaitable object has functions that defines the wait behavior. 
    // There is also std::suspend_never and users can also define their own awaitable objects.
    std::suspend_always initial_suspend()
    {
        return {};
    }

    // Whether coroutine state is destroyed at last suspension point.
    std::suspend_always final_suspend() noexcept
    {
        return {};
    }

    void unhandled_exception() noexcept
    {
        std::rethrow_exception(std::current_exception());
    }

    // This enables the 'co_yield value;' syntax.
    // The type of 'value' dictates the return type of co_yield later in the function.
    std::suspend_always yield_value(T value)
    {
        m_value = value;
        return {};
    }

    // Necessary when coroutine returns something.
    // It enables the 'co_return value;' syntax.
    // The type of 'value' dictates the return type of co_return later in the function.
    // 
    // NOTE: 'return_void' cannot co-exist with 'return_value'.
    void return_value(T value)
    {
        m_value = value;
    }

    // Necessary when coroutine doesn't return anything.
    // It enables the 'co_return;' syntax (which is optional).
    // 
    // NOTE: 'return_void' cannot co-exist with 'return_value'.
    //void return_void()
    //{
    //}

    T m_value = 0;
};

// -------------------------
// Let's create functions and make them coroutines.

// What makes functions a coroutine is two things:
// 1. It uses at least one coroutine keyword: co_await, co_yield or co_return.
// 2. The return type is a coroutine type (uses coroutine infrastructure).
CoroutineType func1()
{
    printf("Doing first thing...\n");
    co_await std::suspend_always{}; // Suspension point #1

    printf("Doing second thing...\n");
    co_await std::suspend_always{}; // Suspension point #2

    printf("Doing third thing...\n");
    co_return 0;
}

// Generates numbers indefinitely.
// This is a common usage of coroutines, the coroutine is like a number generator
// and each time the function is called it'll generate the next number.
CoroutineType func2()
{
    int start = 1;
    while (true)
    {
        co_yield start++;
    }
    co_return 0;
}

CoroutineType func3()
{
    co_yield 45;
    co_yield 46;
    co_yield 47;
    co_return 48;
}

void Coroutines()
{
    // f1 is an instance of CoroutineType and it's got the handle to the coroutine.
    // This won't execute any statement of func1, this is because promise_type::initial_suspend()
    // implementation returns std::suspend_always.
    auto f1 = func1();

    f1.m_handle.resume(); // Runs and hits suspension point #1
    printf("Is func1 done? %s\n", f1.m_handle.done() ? "YES" : "NO"); // prints "NO"
    f1.m_handle.resume(); // Runs and hits suspension point #2
    f1.m_handle.resume(); // Runs and hits end of coroutine.
    printf("Is func1 done? %s\n", f1.m_handle.done() ? "YES" : "NO"); // prints "YES"
    //f1.m_handle.resume(); // Error! Trying to resume an ended coroutine.
    printf("\n");

    auto f2 = func2();
    for (int i = 0; i < 10; ++i) // We just take 10 elements from func2
    {
        // IMPORTANT: Notice numbers are generated lazily, on the fly as we need them.
        f2.m_handle(); // Same as doing 'f2.m_handle.resume();'
        printf("%d,", f2.m_handle.promise().m_value); // 1,2,3,4,5,6,7,8,9,10,
    }
    printf("\n");
    printf("Is func2 done? %s\n", f2.m_handle.done() ? "YES" : "NO"); // prints "NO"
    printf("\n");

    // Helper lambda to resume coroutine and return value directly.
    // CoroutineType could implement this as a member function.
    auto resume = [](CoroutineType& f)
        {
            f.m_handle.resume();
            return f.m_handle.promise().m_value;
        };

    auto f3 = func3();
    printf("%d\n", resume(f3)); // 45
    printf("%d\n", resume(f3)); // 46
    printf("%d\n", resume(f3)); // 47
    printf("%d\n", resume(f3)); // 48
    printf("Is func3 done? %s\n", f3.m_handle.done() ? "YES" : "NO"); // prints "YES"
    printf("\n");

    printf("Coroutines Done!\n");
    printf("\n");
}

// --------------------------------------------------------------------------------
// Modules
// --------------------------------------------------------------------------------
