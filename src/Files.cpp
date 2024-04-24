#include <string>
#include <inttypes.h>

// ---------------
// C FILE
// ---------------

#include <cstdio>

void File()
{
    printf("--------------------------------\n");
    printf("C FILE\n");
    printf("--------------------------------\n");

    // Create and write a file
    // Flags:
    // - "a" can be used to append at the end
    // - Adding "b" will write/read to a binary file instead of text
    if (FILE* outFile = nullptr;
        fopen_s(&outFile, "FileExample.txt", "w") == 0)
    {
        const char data[] = "Another world!\n";
        fwrite(data, sizeof(char), sizeof(data) - 1, outFile); // Write "sizeof(data) - 1" number of chars ("sizeof(char)") into the file.

        fflush(outFile); // Perform any operations that migh be remaining

        long filePosition = ftell(outFile);
        fseek(outFile, filePosition - 3, SEEK_SET);

        fprintf(outFile, ", this is C!\n");

        fseek(outFile, 0, SEEK_SET);  // Go to begining of file. rewind(outFile) would be the same
        fseek(outFile, 0, SEEK_END);  // Go to end of file

        fprintf(outFile, "Bye\n");

        fclose(outFile);
    }
    printf("\n");

    // Read a file
    if (FILE* inFile = nullptr;
        fopen_s(&inFile, "FileExample.txt", "r") == 0)
    {
        char line[256] = { 0 };
        while (fgets(line, sizeof(line) - 1, inFile) != NULL)
        {
            // fgets will include the \n already.
            printf("%s", line);
        }

        rewind(inFile);

        char buffer[256] = { 0 };
        fread(buffer, sizeof(char), sizeof(buffer) - 1, inFile);
        printf("%s\n", buffer);

        printf("Error: 0x%X\n", ferror(inFile));
        printf("EOF: %s\n", feof(inFile) ? "YES" : "NO");

        fclose(inFile);
    }
    printf("\n");

    // Read and write a file
    // - Pass "r+" to fopen. The file must exists.
    // - Pass "w+" to fopen. The file is created if it doesn't exists.
}

// ----------------
// C++ file streams
// ----------------

#include <fstream>

void FileStreams()
{
    printf("--------------------------------\n");
    printf("C++ file streams\n");
    printf("--------------------------------\n");

    // Create and write a file
    // Flags:
    // - std::ofstream::app can be used to append at the end
    // - std::ofstream::binary can be used to write to a binary file instead of text
    if (std::ofstream outFile("FileStreamExample.txt", std::ofstream::out);
        outFile.is_open())
    {
        outFile << "Hello, world!" << std::endl;

        const char data[] = "Another world!\n";
        outFile.write(data, sizeof(data)-1); // Write bytes (const char*) into the file.

        outFile.flush(); // Perform any operations that migh be remaining

        std::streampos filePosition = outFile.tellp();
        outFile.seekp(filePosition - std::streamoff(3));

        const char data2[] = ", this is C++!\n";
        outFile.write(data2, sizeof(data2)-1);

        outFile.seekp(std::streamoff(0), std::ios_base::beg); // Go to begining of file
        outFile.seekp(std::streamoff(0), std::ios_base::end); // Go to end of file

        outFile.write("Bye\n", 4);

        outFile.close();
    }
    printf("\n");

    // Read a file
    if (std::ifstream inFile("FileStreamExample.txt", std::ifstream::in); // std::ifstream::ate can be used to open pointing at the end of the file
        inFile.is_open())
    {
        char line[256] = {0};
        while (inFile.getline(line, sizeof(line)-1))
        {
            // getline will ignore the \n at the end
            printf("%s\n", line);
        }
        inFile.clear(); // Clear error state set by the last getline() call

        inFile.seekg(std::streamoff(0), std::ios_base::beg);

        std::string lineString;
        while (std::getline(inFile, lineString))
        {
            printf("%s\n", lineString.c_str());
        }
        inFile.clear(); // Clear error state set by the last getline() call

        inFile.seekg(std::streamoff(0), std::ios_base::beg);

        char buffer[256] = { 0 };
        inFile.read(buffer, sizeof(buffer) - 1);
        printf("%s\n", buffer);

        printf("State: 0x%X\n", inFile.rdstate()); // State will not be 0x00 as it reached end of file in last read
        printf("Read Count: %" PRId64  "\n", inFile.gcount());
        printf("EOF: %s\n", inFile.eof() ? "YES" : "NO");

        inFile.close();
    }
    printf("\n");

    // Read and write a file
    // std::fstream can do both read and write into a file by using "std::fstream::in | std::fstream::out"
    // To be able to open it for read and write the file must exist already.
}

// ---------------------
// C++17 std::filesystem
// 
// https://en.cppreference.com/w/cpp/filesystem
// ---------------------

#include <filesystem>

void FileSystem()
{
    printf("--------------------------------\n");
    printf("C++17 std::filesystem\n");
    printf("--------------------------------\n");

    const std::filesystem::path filePath("FileSystemExample.txt");

    if (std::ofstream outFile(filePath, std::ofstream::out);
        outFile.is_open())
    {
        outFile << "New file!" << std::endl;
        outFile.close();
    }

    if (std::filesystem::exists(filePath))
    {
        const auto fileSize = std::filesystem::file_size(filePath);

        printf("File '%s' has a size of %ju bytes!\n", filePath.generic_string().c_str(), fileSize);
        printf("Is path absolute? %s\n", filePath.is_absolute() ? "YES" : "NO");
        printf("Is path relative? %s\n", filePath.is_relative() ? "YES" : "NO");

        std::filesystem::path tmpPath(".");
        if (std::filesystem::space(tmpPath).available > fileSize)
        {
            tmpPath.append("temp");
            printf("Creating directory '%s'!\n", tmpPath.generic_string().c_str());
            std::filesystem::create_directory(tmpPath);

            tmpPath /= filePath.filename(); // Append operator
            printf("Copying '%s' -> '%s'\n", filePath.generic_string().c_str(), tmpPath.generic_string().c_str());
            std::filesystem::copy_file(filePath, tmpPath, std::filesystem::copy_options::overwrite_existing);
        }
    }
}
