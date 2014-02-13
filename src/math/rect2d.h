#pragma once

template <typename T>
class rect2d
{
public:
    rect2d()
    {
        this->x=0;
        this->y=0;
        this->w=0;
        this->h=0;

        calculate_bounds();
    }
    rect2d(T x, T y, T w, T h)
    {
        this->x=x;
        this->y=y;
        this->w=w;
        this->h=h;

        calculate_bounds();
    }
    ~rect2d() {};
    bool is_point_inside(T x, T y)
    {
        return x>=this->x&&y>=this->y&&x<=this->x2&&y<=this->y2;
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

    rect2d<T> shrink(T w, T h)
    {
        rect2d<T> ret=*this;
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

    rect2d<T> shrink(T s)
    {
        return shrink(s,s);
    }

    std::string to_string()
    {
        char buf[256];
        sprintf(buf,"RECT: %d %d %d %d\n",x,y,w,h);
        return std::string(buf);
    }

    void clip(rect2d &other)
    {
        if(x+w>=other.w)
            x=other.w-w;
        if(x<=0)
            x=0;
        if(y+h>=other.h)
            y=other.h-h;
        if(y<=0)
            y=0;
    }
    template <typename B>
    rect2d<B> as()
    {
        return rect2d<B>(x,y,w,h);
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
