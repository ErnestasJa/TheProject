#ifndef PLANE3D_H_INCLUDED
#define PLANE3D_H_INCLUDED

class Plane3d
{
public:
    ///  pB--------pD
    ///  |       /  |
    ///  |     /    |
    ///  |   /      |
    ///  | /        |
    ///  pA--------pC

    Plane3d()
    {
        n=glm::normalize(glm::vec3(1));
        D=0.f;
    }

    ~Plane3d()
    {

    }

    void SetPoints(const glm::vec3 &pA, const glm::vec3 &pB, const glm::vec3 &pC)
    {
        glm::vec3 v1=pB-pA;
        glm::vec3 v2=pC-pA;
        glm::vec3 n=glm::cross(v1,v2);
        n=glm::normalize(n);

        D=glm::dot(-n,pA);

        points[0]=pA;
        points[1]=pB;
        points[2]=pC;
        points[3]=glm::cross(pB,pC);
    }

    const glm::vec3 *GetPoints()
    {
        return points;
    }

    float Distance(const glm::vec3 &p)
    {
        return glm::dot(n,p)+D;
    }
private:
    glm::vec3 points[4];
    glm::vec3 n;
    float D;
protected:
};

#endif // PLANE3D_H_INCLUDED
