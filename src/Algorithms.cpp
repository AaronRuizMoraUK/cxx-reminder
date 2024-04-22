#include <vector>
#include <string>
#include <format>
#include <numeric>
#include <execution>


namespace
{
    template<class T>
    void PrintContainer(const T& container)
    {
        for (const auto& element : container)
        {
            std::printf("%d ", element);
        }
    }
}

// Characteristic to clasify the algorithms:
// - Index Viewed: 1 Index means 1 lookup in the range. 2 Index means 2 lookups in the range (current and next).
// - Acumulator: whether the algorithm uses an accumulator variable or not.
// - Reduce/Transform: Refers to kind of outcome expected, another range of same size (Transform) or final structure/value from iterating the range (reduce).

// --------------------
// 1 Index accumulators
// --------------------

// Indexes Viewed: 1
// Accumulator: YES
// Operation: Reduce
void Reduce()
{
    const std::vector<int> numbers = {2, 6, 1, 5, 34, 12, 65, 21};
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    // Accumulator can be ANY type. Returns accumulator's type.
    // Default accumulator operator: std::plus<T>()
    // 
    // result = initial + element1 + element2 + ...

    int sum = std::accumulate(numbers.cbegin(), numbers.cend(), 0 /*accumulator (int)*/);
    std::printf("std::accumulate: %d\n", sum);

    int product = std::accumulate(numbers.cbegin(), numbers.cend(), 1 /*accumulator (int)*/, std::multiplies<int>() /*Reduce Operator*/);
    std::printf("std::accumulate: %d\n", product);

    std::string result = std::accumulate(numbers.cbegin(), numbers.cend(),
        // Accumulator is a string
        std::string("All numbers: "),
        // Reduce operator takes (accumulator, element) as input paramters
        [](std::string accumulator, int element)
        {
            return std::move(accumulator) + std::to_string(element) + " ,";
        });
    std::printf("std::accumulate: %s\n", result.c_str());

    // -------------------------------
    // std::reduce is the same but it supports parallel execution, so the operation performs needs
    // to be associative and commutative as the order of execution might not be sequential.
    int sum1 = std::reduce(numbers.cbegin(), numbers.cend(), 0);
    std::printf("std::reduce: %d\n", sum1);
    int sum2 = std::reduce(std::execution::seq, numbers.cbegin(), numbers.cend(), 0); // Sequential
    std::printf("std::reduce: %d\n", sum2);
    int sum3 = std::reduce(std::execution::par, numbers.cbegin(), numbers.cend(), 0); // In parallel
    std::printf("std::reduce: %d\n", sum3);
}

// Indexes Viewed: 1 (but may be looking at more than one range)
// Accumulator: YES
// Operation: Reduce
void TransformReduce()
{
    const std::vector<int> numbers1 = { 2, 6, 1, 5, 34, 12, 65, 21 };
    const std::vector<int> numbers2 = { 2, 2, 2, 2, 2, 2, 2, 2 };
    std::printf("Input1: ");
    PrintContainer(numbers1);
    std::printf("\n");
    std::printf("Input2: ");
    PrintContainer(numbers2);
    std::printf("\n");

    // Accumulator can be ANY type. Returns accumulator's type.
    // Default accumulator operator: std::plus<T>()
    // Default operator between 2 ranges: std::multiplication<T>()
    // 
    // result = initial + (range1.element1 * range2.element1) + (range1.element2 * range2.element2) + ...

    int sum = std::inner_product(numbers1.cbegin(), numbers1.cend(), numbers2.cbegin() /*where to start in range 2*/, 0 /*accumulator (int)*/);
    std::printf("std::inner_product: %d\n", sum);

    std::string result = std::inner_product(numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(),
        // Accumulator is a string
        std::string("All numbers: "),
        // Reduce operator takes (accumulator, transformedElement) as input paramters
        // Note: transformedElement is the returned value from the transform operator (next parameter)
        [](std::string accumulator, int transformedElement)
        {
            return std::move(accumulator) + std::to_string(transformedElement) + " ,";
        },
        // Transform operator takes (elementRange1, elementRange2) as input parameters
        // Note: this returns the transformed element and it can be of any type. For example std::equals_to() works and returns a boolean.
        [](int elementRange1, int elementRange2)
        {
            return elementRange1 * elementRange2;
        });
    std::printf("std::inner_product: %s\n", result.c_str());

    // -------------------------------
    // std::transform_reduce is the same but it supports parallel execution, so the operations performs needs
    // to be associative and commutative as the order of execution might not be sequential.
    int sum1 = std::transform_reduce(numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(), 0);
    std::printf("std::transform_reduce: %d\n", sum1);
    int sum2 = std::transform_reduce(std::execution::seq, numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(), 0); // Sequential
    std::printf("std::transform_reduce: %d\n", sum2);
    int sum3 = std::transform_reduce(std::execution::par, numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(), 0); // In parallel
    std::printf("std::transform_reduce: %d\n", sum3);
}

// Indexes Viewed: 1
// Accumulator: YES
// Operation: Transform
void InclusiveScan_ExclusiveScan()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    std::vector<int> transformedNumbers(numbers.size());

    // Accumulator is created with the first element of the range, not specified in parameters.
    // Then transforms in order each element of the range (starting from the second) with
    // the accumulator, each operation is stored in both the element and the accumulator.
    // Default accumulator operator: std::plus<T>()

    std::partial_sum(numbers.cbegin(), numbers.cend(), transformedNumbers.begin());
    std::printf("std::partial_sum: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // NOTE: Alternatively, transformedNumbers could be empty and std::back_inserter(transformedNumbers)
    // could be passed to partial_sum instead of transformedNumbers.begin(). This way it will automatically
    // add the elements.

    std::partial_sum(numbers.cbegin(), numbers.cend(), transformedNumbers.begin(),
        // Transform operator takes (accumulator, element) as input paramters
        [](int accumulator, int element)
        {
            return accumulator * element;
        });
    std::printf("std::partial_sum: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // --------------------------------------------------------
    // std::inclusive_scan is the same but it also supports parallel execution, so the operation performs needs
    // to be associative and commutative as the order of execution might not be sequential.
    std::inclusive_scan(numbers.cbegin(), numbers.cend(), transformedNumbers.begin());
    std::printf("std::partial_sum: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // --------------------------------------------------------
    // std::exclusive_scan is the same, but it specifies the initial value of the accumulator for the first
    // element of the output, then it does the same as std::inclusive_scan.
    // Accumulator must be the same element as the range.
    // It also supports parallel execution.
    std::exclusive_scan(numbers.cbegin(), numbers.cend(), transformedNumbers.begin(), 0 /*intial value of accumulator*/);
    std::printf("std::exclusive_scan: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");
}

// ------------------------
// 1 Index non-accumulators
// ------------------------

// Indexes Viewed: 1
// Accumulator: NO
// Operation: Reduce
void Find()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    auto it = std::find(numbers.cbegin(), numbers.cend(), 34);
    std::printf("Value 34 %s\n", it != numbers.end() ? "found" : "not found");

    it = std::find(numbers.cbegin(), numbers.cend(), 102);
    std::printf("Value 102 %s\n", it != numbers.end() ? "found" : "not found");

    // --------------------------------------------------------
    // There are also the parallel execution versions.
}

// Indexes Viewed: 1 (but may be looking at more than one range)
// Accumulator: NO
// Type: Transform
void Transform()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    std::vector<int> transformedNumbers(numbers.size());

    // Transform each element of input and puts it in the output's element.
    // There is no default operation, unary operator must be specified.
    std::transform(numbers.cbegin(), numbers.cend(), transformedNumbers.begin(),
        // Transform operation
        [](int element)
        {
            return element * 2;
        });
    std::printf("std::transform: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // --------------------------------------------------------
    // std::transform also works with looking at more than 1 range
    const std::vector<int> numbers1 = { 2, 6, 1, 5, 34, 12, 65, 21 };
    const std::vector<int> numbers2 = { 2, 2, 2, 2, 2, 2, 2, 2 };
    std::printf("Input1: ");
    PrintContainer(numbers1);
    std::printf("\n");
    std::printf("Input2: ");
    PrintContainer(numbers2);
    std::printf("\n");

    std::transform(numbers1.cbegin(), numbers1.cend(), numbers2.cbegin(), transformedNumbers.begin(),
        // Binary Transform operation
        [](int elementRange1, int elementRange2)
        {
            return elementRange1 * elementRange2;
        });
    std::printf("std::transform: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // --------------------------------------------------------
    // There are also the parallel execution versions.
}

// --------------------
// 2 Index accumulators
// --------------------

// adjacent_reduce doesn't exist in STL, but it can be written
// as an specialization of transform_reduce.
template<class I, class A, class R, class T>
auto adjacent_reduce(I begin, I end, A initAcc, R reduce, T transform)
{
    I beingCopy = begin;
    return std::transform_reduce(begin, --end, ++beingCopy, initAcc, reduce, transform);
}

// Indexes Viewed: 2
// Accumulator: YES
// Operation: Reduce
void AdjacentReduce()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    // Accumulator can be ANY type. Returns accumulator's type.
    // No default accumulator operator.
    // No default operator between current and next element.
    // 
    // result = initial + (element1 * element2) + (element2 * element3) + ...

    std::string result = adjacent_reduce(numbers.cbegin(), numbers.cend(),
        // Accumulator is a string
        std::string("All numbers: "),
        // Reduce operator takes (accumulator, element) as input paramters
        [](std::string accumulator, int transformedElement)
        {
            return std::move(accumulator) + std::to_string(transformedElement) + " ,";
        },
        // Transform operator takes (elementRange1, elementRange2) as input parameters
        [](int currentElement, int nextElement)
        {
            return currentElement * nextElement;
        });
    std::printf("std::adjacent_reduce: %s\n", result.c_str());
}

// Indexes Viewed: 2
// Accumulator: YES
// Operation: Transform
void TransformInclusiveScan_TransformExclusiveScan()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    std::vector<int> transformedNumbers(numbers.size());

    // Accumulator is created with the first element transformed, not specified in parameters.
    // Then transforms in order each element of the range (starting from the second) and then
    // then does another operation with the accumulator, each operation is stored in both the element and the accumulator.
    // No default operators.

    std::transform_inclusive_scan(numbers.cbegin(), numbers.cend(), transformedNumbers.begin(),
        // Transform accumulator and transformed current element
        [](int accumulator, int transformedElement)
        {
            return accumulator + transformedElement;
        },
        // Transform current element
        [](int currentElement)
        {
            return currentElement * 2;
        });
    std::printf("std::transform_inclusive_scan: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // --------------------------------------------------------
    // There are also the parallel execution versions.

    // std::transform_exclusive_scan is the same, but it specifies the initial value of the accumulator for the first
    // element of the output, then it does the same as std::transform_inclusive_scan.
    // Accumulator must be the same element as the range.
    // It also supports parallel execution.

    std::transform_exclusive_scan(numbers.cbegin(), numbers.cend(), transformedNumbers.begin(),
        // Initial accumulator (won't be transformed)
        1,
        // Transform accumulator and transformed current element
        [](int accumulator, int transformedElement)
        {
            return accumulator + transformedElement;
        },
        // Transform current element
        [](int currentElement)
        {
            return currentElement * 2;
        });
    std::printf("std::transform_exclusive_scan: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");
}

// ------------------------
// 2 Index non-accumulators
// ------------------------
// 
// Indexes Viewed: 2
// Accumulator: NO
// Operation: Reduce
void AdjacentFind()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    // Searches for 2 consecutives elements that satisfy the comparison operator.
    // Default comparison operator: std::equal_to()

    auto it = std::adjacent_find(numbers.begin(), numbers.end());
    std::printf("Did it find 2 consecutives elements that are equal? %s\n", it != numbers.end() ? "YES" : "NO");

    it = std::adjacent_find(numbers.begin(), numbers.end(),
        // Binary operator to compare 2 consecutive elements
        [](int element1, int element2)
        {
            return element1 == element2 / 5; // Stops when finds one element that is 5 times smaller than its consecutive.
        });
    std::printf("Did it find 2 consecutives elements where first is 5 times smaller than the second? %s\n", it != numbers.end() ? "YES" : "NO");

    // --------------------------------------------------------
    // There are also the parallel execution versions.
}

// Indexes Viewed: 2
// Accumulator: NO
// Operation: Transform
void AdjacentTransform() // A better for the actual operation that is std::adjacent_difference
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    std::vector<int> transformedNumbers(numbers.size());

    // It does an operation of currElement = currElement op prevElement
    // Default operator: std::minus()

    std::adjacent_difference(numbers.cbegin(), numbers.cend(), transformedNumbers.begin());
    std::printf("std::adjacent_difference: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    std::adjacent_difference(numbers.cbegin(), numbers.cend(), transformedNumbers.begin(),
        // Operation to do between 2 consecutive elements
        [](int currElement, int prevElement)
        {
            // Invert current element if it's larger than the previous element.
            //std::printf("(%d > %d) Invert? %s\n", currElement, prevElement, (currElement > prevElement) ? "YES" : "NO");
            return (currElement > prevElement)
                ? -currElement
                : currElement;
        });
    std::printf("std::adjacent_difference: ");
    PrintContainer(transformedNumbers);
    std::printf("\n");

    // --------------------------------------------------------
    // There are also the parallel execution versions.
}

// ----------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------

void OtherAlgorithms()
{
    const std::vector<int> numbers = { 2, 6, 1, 5, 34, 12, 65, 21 };
    std::printf("Input: ");
    PrintContainer(numbers);
    std::printf("\n");

    bool result = std::any_of(numbers.cbegin(), numbers.cend(),
        [](int element)
        {
            return element > 25;
        });
    std::printf("std::any_of elements greater than 25: %s\n", result ? "YES" : "NO");

    result = std::all_of(numbers.cbegin(), numbers.cend(),
        [](int element)
        {
            return element > 25;
        });
    std::printf("std::all_of elements greater than 25: %s\n", result ? "YES" : "NO");

    result = std::none_of(numbers.cbegin(), numbers.cend(),
        [](int element)
        {
            return element < 0;
        });
    std::printf("std::none_of elements are negative: %s\n", result ? "YES" : "NO");

    std::vector<int> iotaOutput(10);
    std::iota(iotaOutput.begin(), iotaOutput.end(), -3); // -3 is first element, then subsequent elements are +1 until the end of the range: -3, -2, -1, etc.
    std::printf("std::iota: ");
    PrintContainer(iotaOutput);
    std::printf("\n");
}
