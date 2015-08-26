#pragma once

template <typename T>
class Rect2D
{
public:
    Rect2D()
    {
        this->x=0;
        this->y=0;
        this->w=0;
        this->h=0;

        calculate_bounds();
    }
    Rect2D(T x, T y, T w, T h)
    {
        this->x=x;
        this->y=y;
        this->w=w;
        this->h=h;

        calculate_bounds();
    }
    ~Rect2D() {};
    bool is_point_inside(T ox, T oy)
    {
        return ox>this->x&&oy>this->y&&ox<this->x2&&oy<this->y2;
    }
    void resize(int new_w, int new_h)
    {
        this->w=new_w;
        this->h=new_h;
        calculate_bounds();
    }
    void move(T mx, T my)
    {
        this->x+=mx;
        this->y+=my;
        calculate_bounds();
    }

    void SetPosition(T mx, T my)
    {
        this->x=mx;
        this->y=my;
        calculate_bounds();
    }

    Rect2D<T> shrink(T w, T h)
    {
        Rect2D<T> ret=*this;
        if(ret.w>0+2*w && ret.y2>0+2*h)
        {
            ret.x+=w;
            ret.y+=h;
            ret.w-=2*w;
            ret.h-=2*h;
            ret.calculate_bounds();
        }

        return ret;
    }

    Rect2D<T> shrink(T s)
    {
        return shrink(s,s);
    }

    std::string to_string()
    {
        char buf[256];
        sprintf(buf,"RECT: X %d Y %d X2 %d Y2 %d W %d H %d\n",x,y,x2,y2,w,h);
        return std::string(buf);
    }

    void clip(Rect2D &other)
    {
        if(x+w>=other.w)
            x=other.w-w;
        if(x<=0)
            x=0;
        if(y+h>=other.h)
            y=other.h-h;
        if(y<=0)
            y=0;
        calculate_bounds();
    }
    template <typename B>
    Rect2D<B> as()
    {
        return Rect2D<B>(x,y,w,h);
    }

    T x, y, x2, y2, w, h;
protected:
private:
    void calculate_bounds()
    {
        x2=x+w;
        y2=y+h;
    }
};
