#include <iostream>
#include <cmath>

#pragma once

double pi = 3.14159265358979323846;

int angle(float posx, float posy, float targetx, float targety){
    int deg = atan2(targetx - posx, targety - posy) * 180 / pi;
    if (deg > 90){
        deg = 450 - deg;
    } else {
        deg = 90 - deg;
    }
    return deg;
}

float radians(int angle){
    return angle * pi / 180;
}

float distFrom(float x, float y, float radius, float other_x, float other_y, float other_radius){
    float dist_x = (x + radius) - (other_x + other_radius);
    float dist_y = (y + radius) - (other_y + other_radius);
    float distance = sqrt(pow(dist_x, 2) + pow(dist_y, 2));
    return distance;
}