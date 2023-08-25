#include <iostream>

#include "line.hpp"
#include "vector.hpp"

#pragma once

// Check if a point intersects with a vector of lines.
Line inLine(Vector vec, std::vector<Line> lines){

    for (Line line: lines){
        
        // if distance between both points of line == distance between point1, vec + distance between point2, vec, collision = true
        // (inaccurate to 0.2)
        if (abs(vecDistance(line.point1, line.point2) - (vecDistance(line.point1, vec) +  vecDistance(line.point2, vec))) <= 0.1f){
            return line;
        }

    }
    
    return Line(Vector(0, 1), Vector(0, 1), true);
}