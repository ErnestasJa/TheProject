#ifndef BOX3D_H_INCLUDED
#define BOX3D_H_INCLUDED

class Box3d
{
public:
    Box3d (glm::vec3 pos, glm::vec3 extents)
    {

    }
    ~Box3d()
    {

    }
    glm::vec3 * GetPoint(uint8_t i)
    {
        if(i>8) throw new exception();
        return m_points[i];
    }
private:
protected:
};

#endif // BOX3D_H_INCLUDED
