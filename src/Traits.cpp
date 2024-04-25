
// --------------------------------------------------------------------------------
// Example of how traits are implemented.
// 
// For more details see Item 47 of Effective C++ book:
// https://www.aristeia.com/EC3E/3E_item47.pdf
//
// --------------------------------------------------------------------------------

#include <cstdio>

template<typename T>
struct MyTrait
{
    using MyType = typename T::type_from_class; // Traits force classes to define a type_from_class type
};

class Class1
{
public:
    using type_from_class = double;
};

class Class2
{
public:
    using type_from_class = int;
};

// Function implementation for doubles.
template<typename T>
void FunctionImpl(T var, double) // No need to capture the second argument, which is only used as a function overload mechanism.
{
    printf("Implementation for double\n");
}

// Function implementation for integers.
template<typename T>
void FunctionImpl(T var, int)
{
    printf("Implementation for int\n");
}

// This is the function to call.
template<typename T>
void Function(T var)
{
    FunctionImpl(var, typename MyTrait<T>::MyType()); // Choose what function to call using the trait
}

void Traits()
{
    Class1 class1;
    Function(class1); // Prints "Implementation for double"

    Class2 class2;
    Function(class2); // Prints "Implementation for int"

    printf("\n");
}
