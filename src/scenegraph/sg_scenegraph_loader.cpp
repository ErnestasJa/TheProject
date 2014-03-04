#include "precomp.h"
#include "sg_scenegraph_loader.h"
#include "scenegraph.h"

namespace sg
{

sg_scenegraph_loader::sg_scenegraph_loader()
{
    //ctor
}

sg_scenegraph_loader::~sg_scenegraph_loader()
{
    //dtor
}

bool sg_scenegraph_loader::load_scene(scenegraph * sg, const std::string & filename)
{
    tinyxml2::XMLDocument doc;
    uint32_t len; char * buf;
    len = helpers::read(filename, buf);

    if(len==0)
        return false;

    doc.Parse(buf);

    tinyxml2::XMLElement * scene = doc.FirstChildElement("scene");

    tinyxml2::XMLElement * object = scene->FirstChildElement("object");

    while(object)
    {
        if(object->Attribute("type","MESH"))
        {
            if(tinyxml2::XMLElement * file = object->FirstChildElement("iqm_file"))
            {
                std::cout <<"Got object [" << object->Attribute("name") << "], file = " << file->Attribute("name") << std::endl;

                sg::sg_mesh_object_ptr  obj = sg->load_mesh_object(file->Attribute("name"),false);
                sg->add_child(obj);

                if(tinyxml2::XMLElement * pos = object->FirstChildElement("position"))
                {
                    obj->set_position(glm::vec3(pos->FloatAttribute("x"),pos->FloatAttribute("y"),pos->FloatAttribute("z")));
                }

                if(tinyxml2::XMLElement * rot = object->FirstChildElement("rotation"))
                {
                    obj->set_rotation(glm::quat(glm::vec3(rot->FloatAttribute("x"),rot->FloatAttribute("y"),rot->FloatAttribute("z"))));
                }

                if(tinyxml2::XMLElement * scale = object->FirstChildElement("scale"))
                {
                    obj->set_scale(glm::vec3(scale->FloatAttribute("x"),scale->FloatAttribute("y"),scale->FloatAttribute("z")));
                }


            }
        }


        object = object->NextSiblingElement();
    }

    return true;
}

}
