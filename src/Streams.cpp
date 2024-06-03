#include <iostream>

// --------------------------------------------------------------------------------
// Input/Output library
// 
// https://cplusplus.com/reference/iolibrary/
//
// --------------------------------------------------------------------------------

// ------------------------------------
// Basic Stream Classes (<iostream>)
// 
// C++
// istream: Input stream class used for general input operations from various sources like standard input(keyboard), files, etc.
// ostream: Output stream class used for general output operations to various destinations such as standard output(console), files, etc.
// iostream: Stream class that combines both istream and ostream. It's capable of performing both input and output operations.
// 
// These are abstract classes because they depend fundamentally on an abstract mechanism (streambuf) that must be provided
// by derived classes for it to operate. Like in their specialized forms where the buffer is concretely implemented, for example
// fstream (which uses filebuf) or stringstream (which uses stringbuf). New iostream could be created by passing a concrete
// implementation of streambuf.
// ------------------------------------

// ------------------------------------
// Standard Stream Objects
// 
// C++
// cin: Standard input stream (an instance of istream).
// cout: Standard output stream (an instance of ostream).
// cerr: Standard error stream for output (an instance of ostream); typically unbuffered and used for error messages.
// clog: Similar to cerr but buffered.
// 
// C
// printf
// scanf
// ------------------------------------

void StandardStreamObjects()
{
    int input;
    std::cout << "Enter an integer: ";
    std::cin >> input;
    std::cout << "You entered: " << input << std::endl;

    std::cerr << "This is an error message." << std::endl;
    std::clog << "This is a log message." << std::endl;

    // In C
    int input2;
    printf("Enter another integer: ");
    scanf_s("%d", &input2);
    printf("You entered: %d\n", input2);

    printf("\n");
}

// ------------------------------------
// String Stream Classes (<sstream>)
// 
// C++
// istringstream - A stream class derived from istream that reads from string objects.
// ostringstream - Derived from ostream, used for output to strings.It allows strings to be used as the buffer for output operations.
// stringstream - Combines istringstream and ostringstream; capable of both reading from and writing to strings.
// 
// C
// sprintf
// sscanf
// ------------------------------------

#include <sstream>

void StringStreams()
{
    // Output to string
    std::ostringstream outStream;
    outStream << "Number: " << 42;
    std::string str = outStream.str();
    std::cout << str << std::endl;

    // Input from string
    std::istringstream inStream("123 456");
    int a, b;
    inStream >> a >> b;
    std::cout << "a: " << a << ", b: " << b << std::endl;

    // stringstream can use both input and output API

    // In C
    char outString[256];
    sprintf_s(outString, "Number: 42");
    printf(outString);

    char inString[] = "Number: 123 456";
    int a2, b2;
    sscanf_s(inString, "Number: %d %d", &a2, &b2);
    printf("a2: %d b2: %d", a2, b2);

    printf("\n");
}

// ------------------------------------
// File Stream Classes (<fstream>)
// 
// C++
// ifstream - Derived from istream, it's used specifically for input from files.
// ofstream - Derived from ostream, used for output to files.
// fstream - Combines ifstream and ofstream functionality, making it useful for both input and output to files.
// 
// C
// FILE* inFile = fopen("", "r")
// FILE* outFile = fopen("", "w")
// FILE* inOutFile = fopen("", "r+")
// 
// See Files.cpp
// ------------------------------------
