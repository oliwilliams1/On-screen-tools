#pragma once
#include <iostream>
#include <cmath>

class vec3 {
public:
    float x;
    float y;
    float z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float x) : x(x), y(x), z(x) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator*(const vec3& other) const {
        return vec3(x * other.x, y * other.y, z * other.z);
    }

    vec3 operator/(const vec3& other) const {
        // Check if the divisor is zero
        if (other.x == 0.0f || other.y == 0.0f || other.z == 0.0f) {
            throw std::runtime_error("Division by zero");
        }
        return vec3(x / other.x, y / other.y, z / other.z);
    }

    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3& operator-=(const vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vec3& operator*=(const vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    vec3& operator/=(const vec3& other) {
        // check if the divisor is zero
        if (other.x == 0.0f || other.y == 0.0f || other.z == 0.0f) {
            throw std::runtime_error("Division by zero");
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    void print() const {
        std::cout << x << ", " << y << ", " << z << std::endl;
    }

    vec3 normalize() const {
        float length = sqrt(x * x + y * y + z * z);
        return vec3(x / length, y / length, z / length);
    }
};