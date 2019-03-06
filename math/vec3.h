#pragma once

#include "math/math.h"

class vec3
{
public:
    vec3() {}
    vec3(const float x, const float y, const float z)
    {
        e[0] = x;
        e[1] = y;
        e[2] = z;
    }
    constexpr float x() const { return e[0]; }
    constexpr float y() const { return e[1]; }
    constexpr float z() const { return e[2]; }
    constexpr float r() const { return e[0]; }
    constexpr float g() const { return e[1]; }
    constexpr float b() const { return e[2]; }

    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    constexpr float operator[](int i) const { return e[i]; }
    constexpr inline float& operator[](int i) { return e[i]; }
    bool operator==(const vec3& other) const;

    inline vec3& operator+=(const vec3& v2);
    inline vec3& operator-=(const vec3& v2);
    inline vec3& operator*=(const vec3& v2);
    inline vec3& operator/=(const vec3& v2);
    inline vec3& operator*=(const float l);
    inline vec3& operator/=(const float l);

    constexpr float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline float length() const { return sqrtf(squared_length()); }

    inline void make_unit_vector();

private:
    float e[3];
};

inline void vec3::make_unit_vector()
{
    float t = 1 / length();
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
}

inline vec3& vec3::operator+=(const vec3& v2)
{
    e[0] += v2.x();
    e[1] += v2.y();
    e[2] += v2.z();
    return *this;
}

inline vec3& vec3::operator-=(const vec3& v2)
{
    e[0] -= v2.x();
    e[1] -= v2.y();
    e[2] -= v2.z();
    return *this;
}

inline vec3& vec3::operator*=(const vec3& v2)
{
    e[0] *= v2.x();
    e[1] *= v2.y();
    e[2] *= v2.z();
    return *this;
}

inline vec3& vec3::operator/=(const vec3& v2)
{
    e[0] /= v2.x();
    e[1] /= v2.y();
    e[2] /= v2.z();
    return *this;
}

inline vec3& vec3::operator*=(const float l)
{
    e[0] *= l;
    e[1] *= l;
    e[2] *= l;
    return *this;
}

inline vec3& vec3::operator/=(const float l)
{
    e[0] /= l;
    e[1] /= l;
    e[2] /= l;
    return *this;
}

inline vec3 operator+(const vec3& vec1, const vec3& vec2)
{
    return vec3(vec1.x() + vec2.x(), vec1.y() + vec2.y(), vec1.z() + vec2.z());
}

inline vec3 operator-(const vec3& vec1, const vec3& vec2)
{
    return vec3(vec1.x() - vec2.x(), vec1.y() - vec2.y(), vec1.z() - vec2.z());
}

inline vec3 operator*(const vec3& vec1, const vec3& vec2)
{
    return vec3(vec1.x() * vec2.x(), vec1.y() * vec2.y(), vec1.z() * vec2.z());
}

inline vec3 operator*(const vec3& vec1, const float l)
{
    return vec3(vec1.x() * l, vec1.y() * l, vec1.z() * l);
}

inline vec3 operator*(const float l, const vec3& vec1)
{
    return vec1 * l;
}

inline vec3 operator/(const vec3& vec1, const vec3& vec2)
{
    return vec3(vec1.x() / vec2.x(), vec1.y() / vec2.y(), vec1.z() / vec2.z());
}

inline vec3 operator/(const vec3& vec, const float l)
{
    return vec3(vec.x() / l, vec.y() / l, vec.z() / l);
}

inline bool vec3::operator==(const vec3& other) const
{
    if (math::floatEquals(x(), other.x()) && math::floatEquals(y(), other.y()) && math::floatEquals(z(), other.z()))
    {
        return true;
    }
    return false;
}

inline vec3 make_unit_vector(const vec3& v)
{
    return v / v.length();
}

inline float dot(const vec3& vec1, const vec3& vec2)
{
    return vec1.x() * vec2.x() + vec1.y() * vec2.y() + vec1.z() * vec2.z();
}

inline vec3 cross(const vec3& vec1, const vec3& vec2)
{
    return vec3(
        (vec1.y() * vec2.z() - vec1.z() * vec2.y()),
        (-(vec1.x() * vec2.z() - vec1.z() * vec2.x())),
        (vec1.x() * vec2.y() - vec1.y() * vec2.x()));
}
