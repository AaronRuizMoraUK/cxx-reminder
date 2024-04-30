#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <iostream>
#include <ranges>

namespace
{
    struct Point
    {
        int x;
        int y;
    };

    template<typename T>
        requires std::is_same_v<int, typename T::value_type>
    void Print(T& container)
    {
        for (const auto& element : container)
        {
            std::printf("%d ", element);
        }
    }

    template<typename T>
        requires std::is_same_v<Point, typename T::value_type>
    void Print(T& container)
    {
        for (const auto& element : container)
        {
            std::printf("(%d, %d) ", element.x, element.y);
        }
    }

    // Overload Print function for Ranges. Required to specify the type the range contains,
    // for example Print<int>(range).
    // 
    // NOTE: For ranges it cannot use "const T&" because some range views
    // cannot be const to iterate over them, such as std::views::filter. This
    // is because they cache in a member variable the start of the container
    // inside the begin() for efficiency. But that means the view cannot
    // be const. C++20 Ranges are designed always to be taken by forwarding
    // reference, and then iterated as (possibly-modifiable) lvalues.
    //
    // Article with a more detailed explanation: https://quuxplusone.github.io/blog/2023/08/13/non-const-iterable-ranges/
    //
    template<typename U, std::ranges::range T>
    void Print(T&& range)
    {
        // Simple way to print the range by copying it into std::cout with " " between elements.
        std::ranges::copy(range, std::ostream_iterator<U>(std::cout, " "));
    }
}

// --------------------------------------------------------------------------------
// Ranges
// 
// A range is a new feature of C++20 and it's basically an iterable sequence.
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
// X Reduce(); TransformReduceWith2Ranges();        // Index 1 / Accumulator YES / Operation Reduce
// X InclusiveScan_ExclusiveScan();                 // Index 1 / Accumulator YES / Operation Transform
// - Find();                                        // Index 1 / Accumulator NO  / Operation Reduce
// - Transform(); TransformWith2Ranges();           // Index 1 / Accumulator NO  / Operation Transform
// X AdjacentReduce();                              // Index 2 / Accumulator YES / Operation Reduce
// X AdjacentInclusiveScan_AdjacentExclusiveScan(); // Index 2 / Accumulator YES / Operation Transform
// - AdjacentFind();                                // Index 2 / Accumulator NO  / Operation Reduce
// X AdjacentTransform();                           // Index 2 / Accumulator NO  / Operation Transform
// X iota
// 
// --------------------------------------------------------------------------------

void Ranges()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    Print(numbers);
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
    Print(transformedNumbers);
    std::printf("\n");

    // Transform with 2 ranges
    const std::vector<int> numbers2 = { 2, 2, 2, 2, 2, 2, 2, 2 };

    std::ranges::transform(numbers, numbers2, transformedNumbers.begin(),
        // Binary Transform operation
        [](int elementRange1, int elementRange2)
        {
            return elementRange1 * elementRange2;
        });
    std::printf("std::ranges::transform: ");
    Print(transformedNumbers);
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
    Print(numbersDoubled);
    std::printf("\n");

    std::ranges::sort(numbersDoubled);
    std::printf("std::ranges::sort: Input doubled sorted: ");
    Print(numbersDoubled);
    std::printf("\n");

    std::printf("\n");
}

void RangeProjections()
{
    // A projection is a callable function that can be passed to all std::ranges algorithms.
    // The projection receives the element of the container and whatever it returns it's
    // going to be used for the algorithm (instead of the element itself).
 
    // For example, given a Point with X and Y members, sort a container of points by their X.
    const std::vector<Point> points = { {2, 3}, {1, 7}, {8, -2}, {4, 0} };
    std::printf("Input: ");
    Print(points);
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
    Print(pointsSortedByX);
    std::printf("\n");

    pointsSortedByX = points;

    // If the projection is only a member, then there is a simpler syntax by pointing to the member directly like this.
    std::ranges::sort(pointsSortedByX, std::less{}, &Point::x);
    std::printf("std::ranges::sort: Input sorted by X: ");
    Print(pointsSortedByX);
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

    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    Print(numbers);
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
    // passing complicated templates, but the adapters std::views simplify them.
    // ----------------------

    // Transform View
    // 
    // This creates a view that transforms all the elements of the container,
    // for example, multiply the elements by 10. It won't modify the container itself.
    auto numbersTransformView = 
        std::views::transform(numbers, [](int element) { return element * 10; });

    std::printf("std::views::transform 10x: ");
    Print<int>(numbersTransformView); // Computation is done inside Print when iterating over the view.
    std::printf("\n");

    // Take View
    // 
    // This creates a view of the first X elements of the container.
    // For example, first 5 elements.
    auto numbersTakeView = std::views::take(numbers, 5);

    std::printf("std::views::take 5: ");
    Print<int>(numbersTakeView); // Computation is done inside Print when iterating over the view.
    std::printf("\n");

    // Take While View
    // 
    // This creates a view that takes elements as long as the predicate condition is met.
    // For example, take elements while elements are even.
    auto numbersTakeWhileView = 
        std::views::take_while(numbers, [](int element) { return element % 2 == 0; });

    std::printf("std::views::take_while even: ");
    Print<int>(numbersTakeWhileView); // Computation is done inside Print when iterating over the view.
    std::printf("\n");

    // Other useful views:
    // - Drop View (std::views::drop) will drop the first X number of elements.
    // - Drop While View (std::views::drop_while) will drop elements as long as the predicate condition is met.
    // - Keys View (std::views::keys) will get all the key elements of the container (when container is using pair, tuple...)
    // - Values View (std::views::values) will get all the value elements of the container (when container is using pair, tuple...)
    //
    // For the full list C++20 Views see Range Adaptors section in https://en.cppreference.com/w/cpp/ranges

    std::printf("\n");
}

void RangeViewCompositionAndPipeOperator()
{
    // Views can be concatenated to compose much more complex views.
    // All without modifying the original container, as Views do not own the content.

    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    Print(numbers);
    std::printf("\n");

    // For example, filter the even numbers and square them.
    auto myView = std::views::transform(
        std::views::filter(numbers, [](int element) { return element % 2 == 0; }),
        [](int element) { return element * element; });

    std::printf("my view: ");
    Print<int>(myView); // Computation is done inside Print when iterating over the view.
    std::printf("\n");

    // -----------------
    // A much more readable syntax is using the pipe operator.
    // With the pipe operator the views don't need to receive the collection.

    auto myViewPiped = numbers
        | std::views::filter([](int element) { return element % 2 == 0; })
        | std::views::transform([](int element) { return element * element; });

    std::printf("my view piped: ");
    Print<int>(myViewPiped); // Computation is done inside Print when iterating over the view.
    std::printf("\n");

    // -----------------
    // Other cool queries with views

    const std::vector<std::pair<std::string, int>> students = { {"Paco", 15}, {"Lucy", 12}, {"John", 14}, {"Cora", 15} };

    std::printf("Students keys: ");
    Print<std::string>(students | std::views::keys);
    std::printf("\n");

    std::printf("Students values in reverse: ");
    Print<int>(students | std::views::values | std::views::reverse); // NOTE: reverse view works if container support reverse iterators
    std::printf("\n");

    std::printf("Students keys before letter M: ");
    auto beforeM = [](const std::string& name)
        {
            return name.front() < 'M';
        };
    Print<std::string>(students | std::views::keys | std::views::filter(beforeM));
    std::printf("\n");

    std::printf("\n");
}

void RangeFactories()
{
    // There are some view factories that can be used to generate views.

    // Iota Range Factory
    // 
    // std::views::iota generates the range [X, Y)
    auto generatedView = std::views::iota(3, 7); // Nothing computed here!

    std::printf("std::views::iota(3, 7): ");
    Print<int>(generatedView); // Range produced when it's iterated inside the Print!
    std::printf("\n");

    // Alternative usage of iota
    // 
    // Iota generating infinite range starting from 3 (numbers are generate lazily on the fly),
    // then pipe it to a take view to obtain the first 4.
    std::printf("std::views::iota(3) | std::views::take(4): ");
    Print<int>(std::views::iota(3) | std::views::take(4));
    std::printf("\n");

    std::printf("\n");

    // Other range factories:
    // - std::views::empty generates an empty view with no elements.
    // - std::single_view generates a view that contains a single element of a specified value.

    std::printf("std::views::single(3): ");
    Print<int>(std::views::single(3));
    std::printf("\n");
}
