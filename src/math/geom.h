#pragma once

#pragma once

#include "util.h"

struct vec4;
struct vec3;

struct vec2
{
    union
    {
        struct { float x, y; };
        float v[2];
    };

    vec2() {}
    vec2(float x, float y) : x(x), y(y) {}
    explicit vec2(const vec3 &v);
    explicit vec2(const vec4 &v);

    float &operator[](int i)       { return v[i]; }
    float  operator[](int i) const { return v[i]; }

    bool operator==(const vec2 &o) const { return x == o.x && y == o.y; }
    bool operator!=(const vec2 &o) const { return x != o.x || y != o.y; }

    float dot(const vec2 &o) const  { return x*o.x + y*o.y; }
    float squaredlen() const { return dot(*this); }
    float magnitude() const  { return sqrtf(squaredlen()); }
    vec2 &normalize() { mul(1/magnitude()); return *this; }
    float cross(const vec2 &o) const { return x*o.y - y*o.x; }

    vec2 &mul(float f)       { x *= f; y *= f; return *this; }
    vec2 &add(const vec2 &o) { x += o.x; y += o.y; return *this; }
    vec2 &sub(const vec2 &o) { x -= o.x; y -= o.y; return *this; }
    vec2 &neg()              { x = -x; y = -y; return *this; }
};

struct vec3
{
    union
    {
        struct { float x, y, z; };
        float v[3];
    };

    vec3() {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    explicit vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) {}
    explicit vec3(const vec4 &v);

    float &operator[](int i) { return v[i]; }
    float operator[](int i) const { return v[i]; }

    bool operator==(const vec3 &o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const vec3 &o) const { return x != o.x || y != o.y || z != o.z; }

    vec3 operator+(const vec3 &o) const { return vec3(x+o.x, y+o.y, z+o.z); }
    vec3 operator-(const vec3 &o) const { return vec3(x-o.x, y-o.y, z-o.z); }
    vec3 operator+(float k) const { return vec3(x+k, y+k, z+k); }
    vec3 operator-(float k) const { return vec3(x-k, y-k, z-k); }
    vec3 operator-() const { return vec3(-x, -y, -z); }
    vec3 operator*(const vec3 &o) const { return vec3(x*o.x, y*o.y, z*o.z); }
    vec3 operator/(const vec3 &o) const { return vec3(x/o.x, y/o.y, z/o.z); }
    vec3 operator*(float k) const { return vec3(x*k, y*k, z*k); }
    vec3 operator/(float k) const { return vec3(x/k, y/k, z/k); }

    vec3 &operator+=(const vec3 &o) { x += o.x; y += o.y; z += o.z; return *this; }
    vec3 &operator-=(const vec3 &o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    vec3 &operator+=(float k) { x += k; y += k; z += k; return *this; }
    vec3 &operator-=(float k) { x -= k; y -= k; z -= k; return *this; }
    vec3 &operator*=(const vec3 &o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    vec3 &operator/=(const vec3 &o) { x /= o.x; y /= o.y; z /= o.z; return *this; }
    vec3 &operator*=(float k) { x *= k; y *= k; z *= k; return *this; }
    vec3 &operator/=(float k) { x /= k; y /= k; z /= k; return *this; }

    float dot(const vec3 &o) const { return x*o.x + y*o.y + z*o.z; }
    float magnitude() const { return sqrtf(dot(*this)); }
    float squaredlen() const { return dot(*this); }
    float dist(const vec3 &o) const { return (*this - o).magnitude(); }
    vec3 normalize() const { return *this * (1.0f / magnitude()); }
    vec3 cross(const vec3 &o) const { return vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
    vec3 reflect(const vec3 &n) const { return *this - n*2.0f*dot(n); }
    vec3 project(const vec3 &n) const { return *this - n*dot(n); }
};

struct vec4
{
    union
    {
        struct { float x, y, z, w; };
        float v[4];
    };

    vec4() {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    explicit vec4(const vec3 &p, float w = 0) : x(p.x), y(p.y), z(p.z), w(w) {}
    explicit vec4(const float *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

    float &operator[](int i)       { return v[i]; }
    float  operator[](int i) const { return v[i]; }

    bool operator==(const vec4 &o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
    bool operator!=(const vec4 &o) const { return x != o.x || y != o.y || z != o.z || w != o.w; }

    vec4 operator+(const vec4 &o) const { return vec4(x+o.x, y+o.y, z+o.z, w+o.w); }
    vec4 operator-(const vec4 &o) const { return vec4(x-o.x, y-o.y, z-o.z, w-o.w); }
    vec4 operator+(float k) const { return vec4(x+k, y+k, z+k, w+k); }
    vec4 operator-(float k) const { return vec4(x-k, y-k, z-k, w-k); }
    vec4 operator-() const { return vec4(-x, -y, -z, -w); }
    vec4 neg3() const { return vec4(-x, -y, -z, w); }
    vec4 operator*(float k) const { return vec4(x*k, y*k, z*k, w*k); }
    vec4 operator/(float k) const { return vec4(x/k, y/k, z/k, w/k); }
    vec4 addw(float f) const { return vec4(x, y, z, w + f); }

    vec4 &operator+=(const vec4 &o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
    vec4 &operator-=(const vec4 &o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
    vec4 &operator+=(float k) { x += k; y += k; z += k; w += k; return *this; }
    vec4 &operator-=(float k) { x -= k; y -= k; z -= k; w -= k; return *this; }
    vec4 &operator*=(float k) { x *= k; y *= k; z *= k; w *= k; return *this; }
    vec4 &operator/=(float k) { x /= k; y /= k; z /= k; w /= k; return *this; }

    float dot3(const vec4 &o) const { return x*o.x + y*o.y + z*o.z; }
    float dot3(const vec3 &o) const { return x*o.x + y*o.y + z*o.z; }
    float dot(const vec4 &o) const { return dot3(o) + w*o.w; }
    float dot(const vec3 &o) const  { return x*o.x + y*o.y + z*o.z + w; }
    float magnitude() const  { return sqrtf(dot(*this)); }
    float magnitude3() const { return sqrtf(dot3(*this)); }
    vec4 normalize() const { return *this * (1.0f / magnitude()); }
    vec3 cross3(const vec4 &o) const { return vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
    vec3 cross3(const vec3 &o) const { return vec3(y*o.z-z*o.y, z*o.x-x*o.z, x*o.y-y*o.x); }
};

inline vec2::vec2(const vec3 &v) : x(v.x), y(v.y) {}
inline vec2::vec2(const vec4 &v) : x(v.x), y(v.y) {}

inline vec3::vec3(const vec4 &v) : x(v.x), y(v.y), z(v.z) {}

struct mat3x3;
struct mat3x4;

struct quat : vec4
{
    quat() {}
    quat(float x, float y, float z, float w) : vec4(x, y, z, w) {}
    quat(float angle, const vec3 &axis)
    {
        float s = sinf(0.5f*angle);
        x = s*axis.x;
        y = s*axis.y;
        z = s*axis.z;
        w = cosf(0.5f*angle);
    }
    explicit quat(const vec3 &v) : vec4(v.x, v.y, v.z, -sqrtf(max(1.0f - v.squaredlen(), 0.0f))) {}
    explicit quat(const float *v) : vec4(v[0], v[1], v[2], v[3]) {}
    explicit quat(const mat3x3 &m) { convertmatrix(m); }
    explicit quat(const mat3x4 &m) { convertmatrix(m); }

    void restorew()
    {
        w = -sqrtf(max(1.0f - dot3(*this), 0.0f));
    }

    quat normalize() const { return *this * (1.0f / magnitude()); }

    quat operator*(float k) const { return quat(x*k, y*k, z*k, w*k); }
    quat &operator*=(float k) { return (*this = *this * k); }

    quat operator*(const quat &o) const
    {
        return quat(w*o.x + x*o.w + y*o.z - z*o.y,
                    w*o.y - x*o.z + y*o.w + z*o.x,
                    w*o.z + x*o.y - y*o.x + z*o.w,
                    w*o.w - x*o.x - y*o.y - z*o.z);
    }
    quat &operator*=(const quat &o) { return (*this = *this * o); }

    quat operator+(const vec4 &o) const { return quat(x+o.x, y+o.y, z+o.z, w+o.w); }
    quat &operator+=(const vec4 &o) { return (*this = *this + o); }
    quat operator-(const vec4 &o) const { return quat(x-o.x, y-o.y, z-o.z, w-o.w); }
    quat &operator-=(const vec4 &o) { return (*this = *this - o); }

    quat operator-() const { return quat(-x, -y, -z, w); }

    void flip() { x = -x; y = -y; z = -z; w = -w; }

    vec3 transform(const vec3 &p) const
    {
        return p + cross3(cross3(p) + p*w)*2.0f;
    }

    void calcangleaxis(float &angle, vec3 &axis)
    {
        float rr = dot3(*this);
        if(rr > 0)
        {
            angle = 2*acosf(w);
            axis = vec3(*this) * (1 / rr);
        }
        else { angle = 0; axis = vec3(0, 0, 1); }
    }

    template<class M>
    void convertmatrix(const M &m)
    {
        float trace = m.a.x + m.b.y + m.c.z;
        if(trace>0)
        {
            float r = sqrtf(1 + trace), inv = 0.5f/r;
            w = 0.5f*r;
            x = (m.c.y - m.b.z)*inv;
            y = (m.a.z - m.c.x)*inv;
            z = (m.b.x - m.a.y)*inv;
        }
        else if(m.a.x > m.b.y && m.a.x > m.c.z)
        {
            float r = sqrtf(1 + m.a.x - m.b.y - m.c.z), inv = 0.5f/r;
            x = 0.5f*r;
            y = (m.b.x + m.a.y)*inv;
            z = (m.a.z + m.c.x)*inv;
            w = (m.c.y - m.b.z)*inv;
        }
        else if(m.b.y > m.c.z)
        {
            float r = sqrtf(1 + m.b.y - m.a.x - m.c.z), inv = 0.5f/r;
            x = (m.b.x + m.a.y)*inv;
            y = 0.5f*r;
            z = (m.c.y + m.b.z)*inv;
            w = (m.a.z - m.c.x)*inv;
        }
        else
        {
            float r = sqrtf(1 + m.c.z - m.a.x - m.b.y), inv = 0.5f/r;
            x = (m.a.z + m.c.x)*inv;
            y = (m.c.y + m.b.z)*inv;
            z = 0.5f*r;
            w = (m.b.x - m.a.y)*inv;
        }
    }
};

struct mat3x3
{
    vec3 a, b, c;

    mat3x3() {}
    mat3x3(const vec3 &a, const vec3 &b, const vec3 &c) : a(a), b(b), c(c) {}
    explicit mat3x3(const quat &q) { convertquat(q); }
    explicit mat3x3(const quat &q, const vec3 &scale)
    {
        convertquat(q);
        a *= scale;
        b *= scale;
        c *= scale;
    }

    void convertquat(const quat &q)
    {
        float x = q.x, y = q.y, z = q.z, w = q.w,
              tx = 2*x, ty = 2*y, tz = 2*z,
              txx = tx*x, tyy = ty*y, tzz = tz*z,
              txy = tx*y, txz = tx*z, tyz = ty*z,
              twx = w*tx, twy = w*ty, twz = w*tz;
        a = vec3(1 - (tyy + tzz), txy - twz, txz + twy);
        b = vec3(txy + twz, 1 - (txx + tzz), tyz - twx);
        c = vec3(txz - twy, tyz + twx, 1 - (txx + tyy));
    }

    mat3x3 operator*(const mat3x3 &o) const
    {
        return mat3x3(
            o.a*a.x + o.b*a.y + o.c*a.z,
            o.a*b.x + o.b*b.y + o.c*b.z,
            o.a*c.x + o.b*c.y + o.c*c.z);
    }
    mat3x3 &operator*=(const mat3x3 &o) { return (*this = *this * o); }

    void transpose(const mat3x3 &o)
    {
        a = vec3(o.a.x, o.b.x, o.c.x);
        b = vec3(o.a.y, o.b.y, o.c.y);
        c = vec3(o.a.z, o.b.z, o.c.z);
    }
    void transpose() { transpose(mat3x3(*this)); }

    vec3 transform(const vec3 &o) const { return vec3(a.dot(o), b.dot(o), c.dot(o)); }
    vec3 transposedtransform(const vec3 &o) const { return a*o.x + b*o.y + c*o.z; }
};

struct mat3x4
{
    vec4 a, b, c;

    mat3x4() {}
    mat3x4(const vec4 &a, const vec4 &b, const vec4 &c) : a(a), b(b), c(c) {}
    explicit mat3x4(const mat3x3 &rot, const vec3 &trans)
        : a(vec4(rot.a, trans.x)), b(vec4(rot.b, trans.y)), c(vec4(rot.c, trans.z))
    {
    }
    explicit mat3x4(const quat &rot, const vec3 &trans)
    {
        *this = mat3x4(mat3x3(rot), trans);
    }
    explicit mat3x4(const quat &rot, const vec3 &trans, const vec3 &scale)
    {
        *this = mat3x4(mat3x3(rot, scale), trans);
    }

    mat3x4 operator*(float k) const { return mat3x4(*this) *= k; }
    mat3x4 &operator*=(float k)
    {
        a *= k;
        b *= k;
        c *= k;
        return *this;
    }

    mat3x4 operator+(const mat3x4 &o) const { return mat3x4(*this) += o; }
    mat3x4 &operator+=(const mat3x4 &o)
    {
        a += o.a;
        b += o.b;
        c += o.c;
        return *this;
    }

    void invert(const mat3x4 &o)
    {
        mat3x3 invrot(vec3(o.a.x, o.b.x, o.c.x), vec3(o.a.y, o.b.y, o.c.y), vec3(o.a.z, o.b.z, o.c.z));
        invrot.a /= invrot.a.squaredlen();
        invrot.b /= invrot.b.squaredlen();
        invrot.c /= invrot.c.squaredlen();
        vec3 trans(o.a.w, o.b.w, o.c.w);
        a = vec4(invrot.a, -invrot.a.dot(trans));
        b = vec4(invrot.b, -invrot.b.dot(trans));
        c = vec4(invrot.c, -invrot.c.dot(trans));
    }
    void invert() { invert(mat3x4(*this)); }

    mat3x4 operator*(const mat3x4 &o) const
    {
        return mat3x4(
            (o.a*a.x + o.b*a.y + o.c*a.z).addw(a.w),
            (o.a*b.x + o.b*b.y + o.c*b.z).addw(b.w),
            (o.a*c.x + o.b*c.y + o.c*c.z).addw(c.w));
    }
    mat3x4 &operator*=(const mat3x4 &o) { return (*this = *this * o); }

    vec3 transform(const vec3 &o) const { return vec3(a.dot(o), b.dot(o), c.dot(o)); }
    vec3 transformnormal(const vec3 &o) const { return vec3(a.dot3(o), b.dot3(o), c.dot3(o)); }
};
