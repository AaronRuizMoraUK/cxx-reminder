
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
    using MyType = typename T::type_from_class;
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

template<typename T>
void DoFunction(T var, double)
{
    printf("Implementation for double\n");
}

template<typename T>
void DoFunction(T var, int)
{
    printf("Implementation for int\n");
}

template<typename T>
void Function(T var)
{
    DoFunction(var, typename MyTrait<T>::MyType());
}

void Traits()
{
    Class1 class1;
    Function(class1); // Printf "Implementation for double"

    Class2 class2;
    Function(class2); // Prints "Implementation for int"
}
