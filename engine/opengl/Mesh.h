#ifndef MESH_H
#define MESH_H

#include "AABB.h"
#include "IBufferObject.h"
#include "BufferObject.h"
#include "utility/Vector.h"


typedef vector<glm::mat3x4> frame;
typedef vector<frame> frame_vec;

struct bone
{
    int32_t parent;
    std::string name;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
};
typedef vector<bone> bone_vec;

struct animation_info
{
    std::string name;
    uint32_t start, num;
    float framerate;
    bool loop;
};
typedef vector<animation_info> anim_info_vec;

struct animation
{
    frame_vec       frames;
    frame           current_frame;
    bone_vec        bones;
    anim_info_vec   info;

    void set_frame(uint32_t frame);
    void set_interp_frame(float frame);
};

struct sub_mesh
{
    std::string name;
    std::string material_name;
    uint32_t start, num_indices;
};

template <typename T>
struct Triangle
{
    Triangle()
    {
        points.resize(3);
        normal=glm::vec3(0);
    }
    ~Triangle()
    {
        points.clear();
    }
    vector<T> points;
    glm::vec3 normal;
};

struct IBufferObject;
class Mesh
{
public:
    enum BUFFER_OBJECT_INDEX
    {
        POSITION     = 0,
        TEXCOORD     = 1,
        NORMAL       = 2,
        TANGENT      = 3,
        BLENDINDEXES = 4,
        BLENDWEIGHTS = 5,
        COLOR        = 6,
        INDICES      = 7,
        BUFFER_COUNT,
    };

    uint32_t vao;
    vector<IBufferObject*> buffers;
    vector<sub_mesh>   sub_meshes;
    animation * anim;
    AABB aabb;

    Mesh();
    virtual ~Mesh();

    void Init();
    void DisableEmptyBuffers();

    void Render();
    void Render(uint32_t sub_mesh_index);

    void render_lines();
    void render_triangle_strip();
    void UploadBuffers();

    template <typename T, typename U>
    vector<Triangle<T> > GetTriangles();

    template <typename T>
    void HardScale(const glm::vec3& scale);

    template <typename T>
    void HardMove(const glm::vec3& trans);

    template<typename T>
    void UploadBufferSubData(BUFFER_OBJECT_INDEX ind,vector<T> subdata,uint32_t offset);

    void free();

    template <class T>
    void RecalculateAABB();
};

template <typename T,typename U>
vector<Triangle<T> > Mesh::GetTriangles()
{
    vector<Triangle<T> > vec;
    Triangle<T> tri;

    BufferObject<T>* vbo=(BufferObject<T>*)this->buffers[Mesh::POSITION];
    IndexBufferObject<U>* ibo=(IndexBufferObject<U>*)this->buffers[Mesh::INDICES];

    auto ibodata=ibo->data;
    auto vbodata=vbo->data;

    vec.resize(ibo->data.size()/3);

    for(uint32_t i=0; i<ibo->data.size(); i+=3)
    {
        tri.points[0]=vbodata[ibodata[i]];
        tri.points[1]=vbodata[ibodata[i+1]];
        tri.points[2]=vbodata[ibodata[i+2]];

        glm::vec3 u=tri.points[1]-tri.points[0];
        glm::vec3 v=tri.points[2]-tri.points[0];

        glm::vec3 norm((u.y*v.z)-(u.z*v.y),
                       (u.z*v.x)-(u.x*v.z),
                       (u.x*v.y)-(u.y*v.x));

        norm=glm::normalize(norm);
        tri.normal=norm;

        vec[i/3]=tri;
    }

    return vec;
}

template <typename T>
void Mesh::HardScale(const glm::vec3& scale)
{
    BufferObject<T> *verts=(BufferObject<T> *)this->buffers[Mesh::POSITION];

    loop(i,verts->data.size())
    {
        verts->data[i]=verts->data[i]*scale;
    }

    UploadBuffers();

    RecalculateAABB<T>();
}

template <typename T>
void Mesh::HardMove(const glm::vec3& trans)
{
    BufferObject<T> *verts=(BufferObject<T> *)this->buffers[Mesh::POSITION];

    loop(i,verts->data.size())
    {
        verts->data[i]=verts->data[i]+trans;
    }

    UploadBuffers();

    RecalculateAABB<T>();
}

template <class T>
void Mesh::RecalculateAABB()
{
    BufferObject<T> * bo = static_cast<BufferObject<T> *>(buffers[0]);

    if(bo->data.size()>0)
    {
        aabb.Reset(static_cast<glm::vec3>(bo->data[0]));

        for(uint32_t i = 1; i < bo->data.size(); i++)
        {
            aabb.AddPoint(static_cast<glm::vec3>(bo->data[i]));
        }
    }
    else
        aabb.Reset(static_cast<glm::vec3>(T()));
}

template <class T>
void Mesh::UploadBufferSubData(BUFFER_OBJECT_INDEX ind,vector<T> subdata,uint32_t offset)
{
    glBindVertexArray(vao);

    for(uint32_t i = 0; i < buffers.size(); i++)
    {
        if(buffers[i])
        {
            if(i==ind)
            {
                BufferObject<T>* bufferino=(BufferObject<T>*)buffers[i];
                bufferino->UploadSubData(subdata,offset);
            }
        }
    }
    glBindVertexArray(0);
}

typedef std::shared_ptr<Mesh> MeshPtr;

#endif // MESH_H
