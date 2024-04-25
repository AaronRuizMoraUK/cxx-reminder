#include <vector>
#include <algorithm>
#include <numeric>

// --------------------------------------------------------------------------------
// Link to C++20 new features cheat-sheet
//
// https://github.com/AaronRuizMoraUK/modern-cpp-features/blob/master/CPP20.md
//
// --------------------------------------------------------------------------------

namespace
{
    struct Point
    {
        int x;
        int y;
    };

    template<typename T>
    void PrintContainer(const T& container)
    {
        for (const auto& element : container)
        {
            std::printf("%d ", element);
        }
    }

    template<>
    void PrintContainer<std::vector<Point>>(const std::vector<Point>& container)
    {
        for (const auto& element : container)
        {
            std::printf("(%d, %d) ", element.x, element.y);
        }
    }
}

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
// 
// A range is just an iterable sequence.
// 
// Algorithms in C++20 come in 2 flavors:
// - Using iterator pairs (std): These are the regular algorithms from C++ that takes begin and
//   end iterators.
// - Using ranges (std::ranges): These will take the container directly, simplifying the usage of
//   the algorithms.
// 
// NOTE: The algorithms in std::ranges are overloaded to be able to pass both the iterator pair
// or the container. Passing the container is simpler, BUT if using iterator pair, prefer the
// std::ranges version over the std version, this because the std::ranges version will use C++20
// concepts to do validations that the iterators passed are correct.
// 
// Not all reduce/transform algorithms (see Algorithms.cpp) have been adapted to ranges in C++20:
// 
// X Reduce(); TransformReduce();                     // Index 1 / Accumulator YES / Operation Reduce
// X InclusiveScan_ExclusiveScan();                   // Index 1 / Accumulator YES / Operation Transform
// - Find();                                          // Index 1 / Accumulator NO  / Operation Reduce
// - Transform();                                     // Index 1 / Accumulator NO  / Operation Transform
// X AdjacentReduce();                                // Index 2 / Accumulator YES / Operation Reduce
// X TransformInclusiveScan_TransformExclusiveScan(); // Index 2 / Accumulator YES / Operation Transform
// - AdjacentFind();                                  // Index 2 / Accumulator NO  / Operation Reduce
// X AdjacentTransform();                             // Index 2 / Accumulator NO  / Operation Transform
// X iota
// 
// --------------------------------------------------------------------------------

#include <ranges>

void Ranges()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    // ----------------------
    // Transform
    std::vector<int> transformedNumbers(numbers.size());

    std::ranges::transform(numbers, transformedNumbers.begin(),
        // Transform operation
        [](int element)
        {
            return element * 2;
        });
    std::printf("std::ranges::transform: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    const std::vector<int> numbers2 = { 2, 2, 2, 2, 2, 2, 2, 2 };

    std::ranges::transform(numbers, numbers2, transformedNumbers.begin(),
        // Binary Transform operation
        [](int elementRange1, int elementRange2)
        {
            return elementRange1 * elementRange2;
        });
    std::printf("std::ranges::transform: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // ----------------------
    // Find
    auto it = std::ranges::find(numbers, 34);
    std::printf("std::ranges:find: Value 34 %s\n", it != numbers.end() ? "found" : "not found");

    // ----------------------
    // AdjacentFind
    it = std::ranges::adjacent_find(numbers);
    std::printf("std::ranges:adjacent_find: Did it find 2 consecutive elements that are equal? %s\n", it != numbers.end() ? "YES" : "NO");

    // ----------------------
    // Other Algorithms
    bool result = std::ranges::any_of(numbers,
        [](int element)
        {
            return element > 25;
        });
    std::printf("std::ranges:any_of elements greater than 25: %s\n", result ? "YES" : "NO");

    result = std::ranges::all_of(numbers,
        [](int element)
        {
            return element > 25;
        });
    std::printf("std::ranges:all_of elements greater than 25: %s\n", result ? "YES" : "NO");

    result = std::ranges::none_of(numbers,
        [](int element)
        {
            return element < 0;
        });
    std::printf("std::ranges:none_of elements are negative: %s\n", result ? "YES" : "NO");

    std::vector<int> numbersDoubled = numbers;
    std::ranges::for_each(numbersDoubled, [](int& element) { element *= 2; });
    std::printf("std::ranges::for_each: Input doubled: ");
    PrintContainer(numbersDoubled);
    std::printf("\n");

    std::ranges::sort(numbersDoubled);
    std::printf("std::ranges::sort: Input doubled sorted: ");
    PrintContainer(numbersDoubled);
    std::printf("\n");

    std::printf("\n");
}

void RangeProjections()
{
    // Projections are callable functions that can be passed to std::ranges algorithms.
    // The projection receives the element of container and whatever it returns it's
    // going to be used for the algorithm, instead of the element itself.
 
    // For example, given a Point with X and Y members, sort a container of points by their X.
    const std::vector<Point> points = { {2, 3}, {1, 7}, {8, -2}, {4, 0} };
    std::printf("Input: ");
    PrintContainer(points);
    std::printf("\n");

    std::vector<Point> pointsSortedByX = points;

    std::ranges::sort(pointsSortedByX,
        [](int x1, int x2) // <-- elements received here are whatever the projection returns.
        {
            return x1 < x2;
        }, 
        [] (const Point& element) // <-- This is the projection, it returns the X member of the point.
        {
            return element.x; // This can return whatever we want.
        });
    std::printf("std::ranges::sort: Input sorted by X: ");
    PrintContainer(pointsSortedByX);
    std::printf("\n");

    pointsSortedByX = points;

    // If the projection is only a member, then you can pass the member directly like this.
    std::ranges::sort(pointsSortedByX, std::less{}, &Point::x);
    std::printf("std::ranges::sort: Input sorted by X: ");
    PrintContainer(pointsSortedByX);
    std::printf("\n");

    // Another example, use std::range::for_each to print only the Y member of points.
    std::printf("std::ranges::for_each: Print Y element of points:");
    std::ranges::for_each(points,
        [](int y)
        {
            printf(" %d", y);
        },
        &Point::y);
    std::printf("\n");

    std::printf("\n");
}

void RangeViews()
{
    // A Range View is a lightweight object that is like a window of a Range, without owning its content.
    //
    // For a list C++20 views, see Range Adaptors section in https://en.cppreference.com/w/cpp/ranges

    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    // Filter View
    // 
    // This creates a view that filters the container.
    // For example, only elements greater than 5 will remain.
    // 
    // IMPORTANT: There is NO computation at this point at all. The computation is done later when
    // using the view.
    std::ranges::filter_view numbersFiltedView = 
        std::ranges::filter_view(numbers, [](int element) { return element >= 5; });

    std::printf("std::ranges::filter_view >=5: ");
    for (const auto& filteredNumber : numbersFiltedView) // <-- IMPORTANT: Computation of the view happens here!
    {
        printf(" %d", filteredNumber);
    }
    std::printf("\n");

    // ----------------------
    // Range Adaptors
    // 
    // An easier way to use views is with its Range Adaptors version.
    //
    // From this:
    //    std::ranges::filter_view numbersFiltedView =
    //        std::ranges::filter_view(...);
    // 
    // To this:
    //    auto numbersFiltedView = std::views::filter(...);
    //
    // Notice now it's using "std::views::filter" and the variable is an auto (which resolves to std::ranges::filter_view).
    // These adaptors in std::views namespace are more intuitive to use. Some views in std::ranges require
    // convoluted templates, but the adapters std::views simplify them.
    // ----------------------

    // Transform View
    // 
    // This creates a view that transforms all the elements of the container,
    // for example multiply the elements by 10. It won't modify the container itself.
    auto numbersTransformView = 
        std::views::transform(numbers, [](int element) { return element * 10; });

    std::printf("std::views::transform 10x: ");
    PrintContainer(numbersTransformView); // Computation is done inside PrintContainer when iterating over the view.
    std::printf("\n");

    // Take View
    // 
    // This creates a view of the first X elements of the container.
    // For example, first 5 elements.
    auto numbersTakeView = std::views::take(numbers, 5);

    std::printf("std::views::take 5: ");
    PrintContainer(numbersTakeView); // Computation is done inside PrintContainer when iterating over the view.
    std::printf("\n");

    // Take While View
    // 
    // This creates a view that takes elements as long as the predicate condition is met.
    // For example, take elements while elements are even.
    auto numbersTakeWhileView = 
        std::views::take_while(numbers, [](int element) { return element % 2 == 0; });

    std::printf("std::views::take_while even: ");
    PrintContainer(numbersTakeWhileView); // Computation is done inside PrintContainer when iterating over the view.
    std::printf("\n");

    // Other views:
    // - Drop View (std::views::drop) will drop the first X number of elements.
    // - Drop While View (std::views::drop_while) will drop elements as long as the predicate condition is met.
    // - Keys View (std::views::keys) will get all the key elements of the container (when container is using pair, tuple...)
    // - Values View (std::views::values) will get all the value elements of the container (when container is using pair, tuple...)

    std::printf("\n");
}

// --------------------------------------------------------------------------------
// Coroutines
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// Modules
// --------------------------------------------------------------------------------
