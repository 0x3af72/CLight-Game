#include <iostream>
#include <vector>

#include "vector.hpp"
#include "vector_functions.hpp"

#pragma once

class Line{
    public:

        // points
        Vector point1;
        Vector point2;

        // empty
        bool empty;

        // constructor
        Line(Vector point1, Vector point2, bool empty);

};

Line::Line(Vector point1, Vector point2, bool empty = false){

    // store points
    this->point1 = point1;
    this->point2 = point2;

    // empty?
    this->empty = empty;

}