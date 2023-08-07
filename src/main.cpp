#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

std::string makeVariableName(const std::string& filename) {
    std::string name = filename;

    // Replace characters that are not allowed in variable names
    std::replace_if(name.begin(), name.end(), [](char c) {
        return !(isalpha(c) || isdigit(c) || c == '_');
    }, '_');

    // If the first character is a digit, prefix with an underscore
    if (isdigit(name[0])) {
        name = '_' + name;
    }

    return name;
}

int main(int argc, char* argv[]) {

    if(argc != 3) {
        std::cout << "ERR: wrong parameters providen\n";
        std::cout << "USAGE: res.exe <output> <resources>\n";
        exit(-1);
    }

    const char* output = argv[1];
    const char* resources = argv[2];

    if(!std::filesystem::is_directory(resources)) {
        std::cout << "ERR: \'" << resources << "\' does not exist or is a file\n";
        exit(-1);
    }

    std::string header = R"(
/*
 *   https://github.com/Wartuu/res-cpp
 *   
 *   MIT License
 *   
 *   Copyright (c) 2023 Wartuu
 *   
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *   
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *   
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */
#ifndef RESOURCE_CPP
#define RESOURCE_CPP

namespace resource {

    )";


    std::vector<std::vector<unsigned char>> files;

    for(const auto& file : std::filesystem::directory_iterator(resources)) {
        if(file.is_directory()) {continue;}

        std::fstream fs(file.path());

        if(!fs.is_open()) {
            std::cout << "ERR: unable to access \'" << file.path() << "\'\n";
            exit(-1);
        }

        fs.seekg(0, std::ios::end);
        std::streampos size = fs.tellg();
        fs.seekg(0, std::ios::beg);




        std::vector<unsigned char> buffer(size);
        fs.read(reinterpret_cast<char*>(buffer.data()), size);

        fs.close();

        header += "unsigned char " + makeVariableName(file.path().filename().string()) + "[] {\n\t";

        std::stringstream ss;

        int counter = 0;
        for(int i = 0; i < buffer.size(); ++i) {

            if(counter == 10) {
                ss << "\n\t";
                counter = 0;
            }

            ss << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);


            if(i < buffer.size() - 1) {
                ss << ", ";
            }

            counter++;
        }

        header += ss.str();
        header += "\n};\n\n";

        std::cout << "added '" << file.path() << "', " << file.file_size() << "b\n";
    }

    header += "};\n\n #endif //RESOURCE_CPP";


    std::ofstream outputStream(output);

    if(!outputStream.is_open()) {
        std::cout << "ERR: failed to open '" << output << "'\n";
        exit(-1);
    }

    outputStream << header;
    outputStream.close();



}