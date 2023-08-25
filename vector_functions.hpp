#include <iostream>
#include <map>
#include <vector>

#include "math_functions.hpp"
#include "vector.hpp"

#pragma once

// Return a vector representing an angle.
Vector vectorFromAngle(int angle){
    float rad = radians(angle);
    return Vector(sin(rad), cos(rad));
}

// Calculate reflected vector given incident vector and surface normal.
// Formula: r = d - 2n(d . n)
Vector calculateReflection(Vector incident_vector, Vector surface_normal){

    // normalize both vectors.
    incident_vector = incident_vector.normalized();
    surface_normal = surface_normal.normalized();

    return incident_vector.minusVector(surface_normal.multiply(2).multiply(incident_vector.dotProduct(surface_normal))).normalized();

}

// Calculate normal of a line through two points.
Vector lineNormal(Vector point1, Vector point2){

    // distances
    float dist_x = point2.x - point1.x;
    float dist_y = point2.y - point1.y;

    // normal
    return Vector(-dist_y, dist_x).normalized();

}

float vecDistance(Vector point1, Vector point2){
    return sqrt(pow(point2.x - point1.x, 2) +  pow(point2.y - point1.y, 2));
}