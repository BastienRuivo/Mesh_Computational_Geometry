#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <math.h>


class vec3
{
public:
    double x;
    double y;
    double z;

    vec3():x(),y(),z() {}
    vec3(float x, float y, float z):x(x),y(y),z(z) {}


    inline vec3 operator+(const vec3 & p) const {
        return vec3(x + p.x, y + p.y, z + p.z);
    }

    inline vec3 operator-(const vec3 & p) const {
        return vec3(x - p.x, y - p.y, z - p.z);
    }

    inline vec3 operator*(const double & d) const {
        return vec3(x * d, y * d, z * d);
    }

    inline vec3 operator/(const double & d) const {
        return vec3(x / d, y / d, z / d);
    }

    inline vec3 operator-() const {
        return vec3(-x, -y, -z);
    }

    inline vec3 operator+=(const vec3 & p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    inline vec3 operator-=(const vec3 & p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }

    inline vec3 operator*=(const double & d) {
        x *= d;
        y *= d;
        z *= d;
        return *this;
    }

    inline vec3 operator/=(const double & d) {
        x /= d;
        y /= d;
        z /= d;
        return *this;
    }

    inline double norm() const {
        return sqrt(x*x + y*y + z*z);
    }

    inline double norm2() const {
        return x*x + y*y + z*z;
    }

    inline vec3 normalize() const {
        return *this / norm();
    }

    inline double dot(const vec3 & p) const {
        return x * p.x + y * p.y + z * p.z;
    }

    inline vec3 cross(const vec3 & p) const {
        return vec3(y * p.z - z * p.y,
                     z * p.x - x * p.z,
                     x * p.y - y * p.x);
    }

    inline vec3 project(const vec3 & p) const {
        return p * (dot(p) / p.norm2());
    }

};

class MathHelper {
public:
    static bool isInTriangle(const vec3 & p, const vec3 & a, const vec3 & b, const vec3 & c){
        vec3 bar = getBarycentric(p, a, b, c);

        if(bar.x >= 0 && bar.y >= 0 && bar.z >= 0 && bar.x <= 1 && bar.y <= 1 && bar.z <= 1) {
            return true;
        }
        return false;
    }
    static vec3 getBarycentric(const vec3 & p, const vec3 & A, const vec3 & B, const vec3 & C) {
        // compute barycentric coordinates
        float aABP = triangleArea(A, B, p);
        float aBCP = triangleArea(B, C, p);
        float aABC = triangleArea(A, B, C);

        float alpha = aBCP / aABC;
        float beta = aABP / aABC;
        float gamma = 1.0f - alpha - beta;

        return vec3(alpha, beta, gamma);
    }
    static float triangleArea(const vec3 & a, const vec3 & b, const vec3 & c) {
        vec3 normal = (b - a).cross(c - a);
        return normal.dot((b - a).cross(c - a)) / 2.0f;
    }
    static int orientation(const vec3 & a, const vec3 & b, const vec3 & c) {
        // return 1 if clockwise, 0 if collinear, -1 if counterclockwise
        float val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
        if (val == 0) return 0;
        return (val > 0)? 1: -1;
    }

};

#endif // MATHHELPER_H
