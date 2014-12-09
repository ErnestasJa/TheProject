#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

class Entity
{
public:
    Entity(uint32_t id)
    {
        this->id=id;
    }
    ~Entity(){}

    uint32_t GetId()
    {
        return id;
    }

    glm::vec3 GetRotation()
    {
        return rot;
    }

    void SetRotation(const glm::vec3 &other)
    {
        rot=other;
    }

    glm::vec3 GetPosition()
    {
        return pos;
    }

    void SetPosition(const glm::vec3 &other)
    {
        pos=other;
    }
private:
protected:
    uint32_t id;
    glm::vec3 pos,rot;
};

#endif // ENTITY_H_INCLUDED
