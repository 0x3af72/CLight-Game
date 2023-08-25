#include <iostream>

#include "math_functions.hpp"

#pragma once

// Vector class implementation.
class Vector{

    public:

        // These represent velocity or position.
        float x;
        float y;

        // Vector constructor.
        Vector(float x, float y);
        Vector() = default;

        // Magnitude: The hypotenuse of the right angled triangle.
        float magnitude();

        // Normalized vector: The same vector but its magnitude is scaled to 1.
        Vector normalized();

        // Dot product: y1 * y2 + x1 * x2
        float dotProduct(Vector other);

        // Multiply by a float.
        Vector multiply(float n);

        // Minus a vector.
        Vector minusVector(Vector other);

};

Vector::Vector(float x, float y){
    this->x = x;
    this->y = y;
}

float Vector::magnitude(){
    return pow(pow(x, 2) + pow(y, 2), 0.5f);
}

Vector Vector::normalized(){
    return Vector(x / magnitude(), y / magnitude());
}

float Vector::dotProduct(Vector other){
    return x * other.x + y * other.y;
}

Vector Vector::multiply(float n){
    return Vector(x * n, y * n);
}

Vector Vector::minusVector(Vector other){
    return Vector(x - other.x, y - other.y);
}