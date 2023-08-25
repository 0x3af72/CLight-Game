#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "vector.hpp"
#include "line.hpp"

#pragma once

/*
Simple header file to load a playground map.

Sample map:
# some line at the top left (comments are ignored)
0 100 100 0
*/

// Split text by delimiter.
std::vector<std::string> _splitText(std::string string, std::string delimiter){
    
    std::vector<std::string> split_text;

    // split
    size_t index;
    while (true){

        // get index and check if its the end
        index = string.find(delimiter);
        if (index == std::string::npos){
            split_text.push_back(string.substr(0, string.length()));
            break;
        }

        split_text.push_back(string.substr(0, index));
        string.erase(0, index + delimiter.size());
    }

    return split_text;

}

// Read the whole of a file and return a std::vector of its lines.
std::vector<std::string> _loadFileContents(std::string path){

    // contents and line declaration
    std::vector<std::string> contents;
    std::string line;

    // read
    std::ifstream reader;
    reader.open(path);
    while (std::getline(reader, line)){
        contents.push_back(line);
    }
    reader.close();

    return contents;
}

// Load a playground map.
std::vector<Line> loadPlayground(std::string path){

    // load file contents
    std::vector<std::string> data = _loadFileContents(path);

    // returned map
    std::vector<Line> playground;
    
    // parse data
    for (std::string line: data){

        // skip comments (start with #)
        if (line.rfind("#", 0) == 0){
            continue;
        }

        // get coordinates
        std::vector<std::string> line_points = _splitText(line, " ");
        
        // atoi the strings
        Line new_line(
            Vector(std::atoi(line_points[0].c_str()), std::atoi(line_points[1].c_str())),
            Vector(std::atoi(line_points[2].c_str()), std::atoi(line_points[3].c_str()))
        );

        playground.push_back(new_line);

    }

    return playground;

}