#pragma once

template <class T>
class vec2
{
public:
    vec2()
    {
        this->x=0;
        this->y=0;
    }
    vec2(T x, T y)
    {
        this->x=x;
        this->y=y;
    }

    vec2<T> operator +(const vec2<T> &b)
    {
        return vec2(this->x+b.x,this->y+b.y);
    }

    vec2<T> operator -(const vec2<T> &b)
    {
        //!Remember: to subtract, first reverse the vector you want to subtract, then add.
        return vec2(this->x+(-b.x),this->y+(-b.y));
    }

    //!Scalar multiplication.
    vec2<T> operator *(const T &a)
    {
        return vec2<T>(this->x*a,this->y*a);
    }

    bool operator ==(const vec2<T> &b)
    {
        return this->x==b.x&&this->y==b.y;
    }

    vec2<T> operator =(const vec2<T> &b)
    {
        this->x=b.x;
        this->y=b.y;
        return *this;
    }

    bool operator !=(const vec2<T> &b)
    {
        return !operator==(b);
    }

    //TODO: 2D Vector multiplication.
    vec2<T> operator *(const vec2<T> &b)
    {

    }

    T x,y;
};
