
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
// Ranges
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Coroutines
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Modules
// --------------------------------------------------------------------------------
