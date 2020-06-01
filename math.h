#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <iostream>
#include <random>

using std::sqrt;

inline float randomf() {
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float randomf(float min, float max) {
    return min + (max-min)*randomf();
}

inline int randomi(int min, int max) {
    return round(min + (max-min)*randomf());
}

const float PI = 3.14159265359f;
const float DEG_TO_RAD = 0.01745329252;

class float3 {
    public:
        float3() : e{0,0,0} {}
        float3(float e0, float e1, float e2) : e{e0, e1, e2} {}

        float x() const { return e[0]; }
        float y() const { return e[1]; }
        float z() const { return e[2]; }

        float3 operator-() const { return float3(-e[0], -e[1], -e[2]); }
        float operator[](int i) const { return e[i]; }
        float& operator[](int i) { return e[i]; }

        float3& operator+=(const float3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        float3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        float3& operator/=(const double t) {
            return *this *= 1/t;
        }

        float length() const {
            return sqrt(squareMagnitude());
        }

        float squareMagnitude() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static float3 Random() {
            return float3(randomf(), randomf(), randomf());
        }

        inline static float3 Random(float min, float max) {
            return float3(randomf(min, max), randomf(min, max), randomf(min, max));
        }

    public:
        float e[3];
};

// Type aliases for float3
using color = float3;
using point3 = float3;
using vec3 = float3;

// float3 Utility Functions
inline std::ostream& operator<<(std::ostream &out, const float3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline float3 operator+(const float3 &u, const float3 &v) {
    return float3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline float3 operator-(const float3 &u, const float3 &v) {
    return float3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline float3 operator*(const float3 &u, const float3 &v) {
    return float3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline float3 operator*(float t, const float3 &v) {
    return float3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline float3 operator*(const float3 &v, float t) {
    return t * v;
}

inline float3 operator/(float3 v, float t) {
    return (1/t) * v;
}

inline double dot(const float3 &u, const float3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline float3 cross(const float3 &u, const float3 &v) {
    return float3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline float3 normalized(float3 v) {
    return v / v.length();
}

inline float saturate(float t) {
    if(t < 0) {
        return 0;
    }
    if(t > 1) {
        return 1;
    }
    return t;
}

inline float lerp(float a, float b, float t) {
    t = saturate(t);
    return a + (b - a) * t;
}

inline float3 lerp(const float3& a, const float3 &b, float t) {
    return float3(lerp(a.x(), b.x(), t), lerp(a.y(), b.y(), t), lerp(a.z(), b.z(), t));
}


//TODO: use proper polar coord random
inline float3 randomInUnitSphere() {
    while(true) {
        auto p = float3::Random(-1, 1);
        if(p.squareMagnitude() >=1 ) {
            continue;
        }
        return p;
    }
}

inline float3 randomOnUnitSphereSurface() {
    auto a = randomf(0, 2*PI);
    auto z = randomf(-1, 1);
    auto r = sqrt(1 - z*z);
    return float3(r*cos(a), r*sin(a), z);
}

inline float3 randomInUnitHemisphere(const float3& N) {
    float3 rand = randomInUnitSphere();
    if (dot(rand, N) > 0.0) 
        return rand;
    else
        return -rand;
}

inline float3 randomOnUnitDisk() {
    while (true) {
        auto p = float3(randomf(-1,1), randomf(-1,1), 0);
        if (p.squareMagnitude() >= 1) continue;
        return p;
    }
}

//from cg reference implementation
inline float3 reflect( const float3 &i, const float3 &n ){
  return i - 2.0 * n * dot(n,i);
}
//from cg reference implementation
inline float3 refract( const float3 &i, const float3 &n, float eta ) {
  float cosi = dot(-i, n);
  float cost2 = 1.0f - eta * eta * (1.0f - cosi*cosi);
  float3 t = eta*i + ((eta*cosi - sqrt(abs(cost2))) * n);
  float dir = cost2 > 0;
  return t * float3(dir, dir, dir);
}

inline float schlick(float cos, float refIndex) {
    auto r0 = (1-refIndex) / (1+refIndex);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cos),5);
}
#endif