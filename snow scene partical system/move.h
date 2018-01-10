#pragma once
#include <cmath>
typedef enum { left, right, front, back }direction;
void outCollisionTest(float* eye, float* center, float x1, float x2, float z1, float z2);
void move(float* eye, float* center, direction dir);
void rotate(float* eye, float* center, direction dir);