#ifndef RECT2D_H
#define RECT2D_H

template <class T>
class rect2d
{
    public:
        rect2d(T x, T y, T w, T h)
        {
            this->x=x;
            this->y=y;
            this->w=w;
            this->h=h;
        }
        ~rect2d();
        bool is_point_inside(T x, T y)
        {
            return x>=this->x&&y>=this->y&&x<=this->x+this->w&&y<=this->y+this->h;
        }
        void resize(int new_w, int new_h)
        {
            this->w=new_w;
            this->h=new_h;
        }
        void move(T mx, T my)
        {
            this->x+=mx;
            this->y+=my;
        }

        T x, y, w, h;
    protected:
    private:
};

#endif // RECT2D_H
