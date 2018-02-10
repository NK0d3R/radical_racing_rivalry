#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include "defs.h"

template <typename type> struct VectorT;
template <typename type> struct LineT;

typedef VectorT<int32_t> Vector;
typedef LineT<int32_t> Line;

struct Rect {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    int16_t max_x;
    int16_t max_y;

    Rect() {
        Set(0, 0, 0, 0);
    };

    Rect(int16_t _x, int16_t _y, int16_t _w, int16_t _h) {
        Set(_x, _y, _w, _h);
    };

    void Set(int16_t _x, int16_t _y, int16_t _w, int16_t _h) {
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        max_x = x + w;
        max_y = y + h;
    };

    void Clip(const Rect& other);
    void ClipLineX(Line& line);
    void ClipPoint(Vector& point);
    bool IsInside(const Vector& point);
    bool IsEmpty() const {return (w == 0 || h == 0);};
};

template <typename type>
struct VectorT 
{
    type x;
    type y;

    VectorT(): x(0), y(0) {
    }

    VectorT(type _x, type _y):x(_x), y(_y) {    
    }

    VectorT& operator += (const VectorT& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    VectorT& operator -= (const VectorT& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    VectorT& operator *= (int32_t scalar)
    {
        x = FROM_FIXED_POINT(x * scalar);
        y = FROM_FIXED_POINT(y * scalar);
        return *this;
    }

    VectorT& operator /= (int32_t scalar)
    {
        x = (TO_FIXED_POINT(x) / scalar);
        y = (TO_FIXED_POINT(y) / scalar);
        return *this;
    }

    VectorT operator + (const VectorT& other)
    {
        return {x + other.x, y + other.y};
    }

    VectorT operator - (const VectorT& other)
    {
        return {x - other.x, y - other.y};
    }

    type magnitudeSQ() { return FROM_FIXED_POINT(x * x + y * y);}
    type magnitude() { return sqrt(x * x + y * y); }
    type dot(const VectorT& other) { 
        return FROM_FIXED_POINT(x * other.x + y * other.y);
    }
    void normalize() { type mag = magnitude(); *this /= mag; }
};

template <typename type>
struct LineT
{
    VectorT<type> start;
    VectorT<type> end;

    LineT(type start_x, type start_y, type end_x, type end_y):
            start(start_x, start_y), end(end_x, end_y) {
    }

    bool Exists() {
        VectorT<type> diff = end - start;
        return (diff.x != 0) || (diff.y != 0);
    }
};

#endif //__PRIMITIVES_H__
