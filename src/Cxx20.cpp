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
// Note that coroutines do not run concurrently, they still use the stack so:
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
// 
// Good coroutine types are hard to build and get right, it's advised to use third party libraries
// that provides them. Github projects that offer a collection of coroutine types:
// - https://github.com/lewissbaker/cppcoro
// - https://github.com/Quuxplusone/coro 

// -------------------------
// Let's create a simple coroutine type using coroutine infrastructure.

template<typename T>
struct Promise;

// Coroutine type with the minimum requirements. It manages the coroutine handle.
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
// 2. The return type is a coroutine type (it uses the coroutine infrastructure).
CoroutineType func1()
{
    printf("Doing first thing...\n");
    co_await std::suspend_always{}; // Suspension point #1

    printf("Doing second thing...\n");
    co_await std::suspend_always{}; // Suspension point #2

    printf("Doing third thing...\n");
    co_return 0;
}

// This coroutine generates numbers indefinitely.
// This is a common usage of coroutines, the coroutine is like a number generator
// and each time the function is resumed it'll generate the next number.
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
    auto resume = [](const CoroutineType& f)
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

// Modules is a new compilation model for C++ to replace header files (.h).
// 
// One of the main advantages of modules is that a module can decide what
// to expose externally or not, improving encapsulation.
// 
// Also, unlike headers, modules won't be compiled each time its included by different
// cpp files. First, modules are compiled into binary, then that binary is used by any
// cpp that imports it. This can improve compilation speed significantly. Each module
// is compiled independently into an object file (translation unit).
//
// Another advantage of modules is that the import order doesn't matter, because the
// module is available in binary format and it's going to be included where needed.
// But this also means that modules need to be compiled in order, which can be a headache
// if building manually by command line, but the use of build tools and IDEs should
// take care of this for us.
//
// Macro definitions inside modules will not have any affect outside the module,
// this can avoid unexpected behavior in the external code when including modules.
//
// Example of a basic Module Interface (.ixx) where it's declaring a Module named 'print':
//
//      // Global Module Fragment
//      module;
//      #include <cstring>   // Old style headers and preprocessor directives are only allowed at this point.
//      #include <string>
// 
//      export module print; // <-- Module declaration with a name
//      
//      // Module Preamble
//      // Section importing other modules required for this module.
//      // These are called "header units" and their compiled Binary Module Interfaces are required
//      // to build this module. Our program will have to build all the modules specified in this section.
//      // C++ system headers can be imported and treated as modules, but depending on
//      // the header some compilers might have issues building them as Binary Module Interfaces.
//      import <iostream>
//      
//      // Module Content (declarations)
//      // Notice the keyword 'export' to indicate what the module is exporting.
//      // The export keyword works also with classes, class templates, variables, namespaces, etc.
//      // Also, an export block can also be used to export multiple functions.
//      export void print_msg(const std::string& msg);
// 
//      // NOTE: Names in .ixx will still be reachable in external code when using auto, even if export
//      //       has not been specified. For example, a non-exported class Point and an exported function
//      //       that returns a Point. External code will be able call the function and capture the return value
//      //       using auto (and even call the class' members), but it won't compile if captured using Point type.
//      //       But for best practices it would be better to mark the class Point as export too.
// 
// This would be the Module Implementation file (.cpp):
// 
//      // Global Module Fragment
//      module;
//      #include <cstring>
//      #include <string>
// 
//      module print; // <-- Notice there is no 'export' keyword
//      
//      // Module Preamble
//      import <iostream>
//      
//      // Module Content (implementations)
//      void print_msg(const std::string& msg)
//      {
//           std::cout << "Msg: " << msg << std::endl;
//      }

// Module file extensions convention unfortunately changes per compiler, although parameters
// can be passed to the compiler to change it:
// 
// MSVC
//      - Module files: (.ixx)
//      - Binary Module Interface: (.ifc)
//      - Module implementation files: (.cpp)
//      - Compiled modules: (.o)
//      - In Visual Studio, apart from being setup to use C++20, remember to set in project properties
//        "C/C++ > General > Scan Sources for Module Dependencies" to Yes. This will first scan all the files
//        for modules dependencies to build the dependency graph so modules are built in the right order.
//
// GCC
//      - Module files - No specific extension, but in many examples: (.cc)
//      - Binary Module Interface: (.gcm)
//      - Module implementation files: (.cpp)
//      - Compiled modules: (.o)
//
// Clang
//      - Module files: (.cppm)
//      - Binary Module Interface: (.pcm)
//          - Clang expects the .pcm filename to be the same as the name of the module indicated inside the .cppm file.
//            Example: If the file 'math.cppm' has 'export module my_math_module;' then the compiled module file needs
//            to be 'my_math_module.pcm' and not 'math.pcm'. But it's a good practice to name the module file and
//            the module the same.
//      - Module implementation files: (.cpp)
//      - Compiled modules: (.o)
//
// Building programs using modules. Let's say we have the following files:
//       - ModuleA.ixx --> module file defining ModuleA
//       - ModuleA.cpp --> module file with ModuleA implementation
//       - ModuleB.ixx --> module file defining ModuleB, which imports and uses ModuleA
//       - ModuleB.cpp --> module file with ModuleB implementation, which imports and uses ModuleA
//       - main.cpp    --> Main program, which imports and uses ModuleB.
// 
// In this scenario, this is the build order of this program:
//       1. Pre-compile ModuleA interface: ModuleA.ixx ---> ModuleA.ifc
//       2. Pre-compile ModuleB interface: ModuleB.ixx --- passing ModuleA.ifc ---> ModuleB.ifc
//       3. Compile ModuleA:               ModuleA.cpp --- passing ModuleA.ifc ---> ModuleA.o
//       4. Compile ModuleB:               ModuleB.cpp --- passing ModuleB.ifc, ModuleA.ifc ---> ModuleB.o
//       5. Compile main:                  main.cpp --- passing ModuleB.ifc ---> main.o
//       6. Link program:                  main.o, ModuleA.o, ModuleB.o ---> main.exe

// Submodules
// In a module interface file (.ixx), if you require another module, then you can add "import OtherModule;", but if
// whoever imports you also needs to be able to access stuff from other module, then you have to
// use "export import OtherModule;" instead.
// 
// This can be used for having modules that it's only a collection of other modules, for example:
//
//      module;
//      export module math;
//
//      export import math.add_sub;  // Submodule. Defined in another file math.add_sub.ixx
//      export import math.mult_div; // Submodule. Defined in another file math.mult_div.ixx
// 
//      // NOTE: The dot is just part of the module's name, there is no hierarchy or accessors here.
//
// So code could choose to "import math;" to import everything, or particular modules they only need, such as "import math.add_sub;"
// Submodules are useful to give flexibility deciding what functionality to import.

// Partitions
// If you still want to split your module but not allow others to import the submodules, then partitions can be used:
//
//      module;
//      export module math;
//
//      export import :add_sub;  // Module partition. Defined in another file math-add_sub.ixx
//      export import :mult_div; // Module partition. Defined in another file math-mult_div.ixx
//
// Then in math-add_sub.ixx:
// 
//      module;
//      export module math:add_sub;
//      ...
//
// Then in math-mult_div.ixx:
// 
//      module;
//      export module math:mult_div;
//      ...
//
// This way, code can do "import math;" but the won't be able to import the parts.
// Partitions are useful to isolate how the module is subdivided.
