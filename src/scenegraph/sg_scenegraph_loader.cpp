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
    std::string scene_path = filename.substr(0,filename.rfind("/")+1);
    std::cout << "scene_path =" << scene_path.c_str() << std::endl;

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
        isg_object * o = nullptr;

        if(object->Attribute("type","MESH"))
        {
            if(tinyxml2::XMLElement * file = object->FirstChildElement("iqm_file"))
            {
                std::cout <<"Got object [" << object->Attribute("name") << "], file = " << file->Attribute("name") << std::endl;

                sg::sg_mesh_object_ptr  obj = sg->load_mesh_object(scene_path+"/"+file->Attribute("name"),true);
                o = obj.get();

                sg->add_object(obj);
            }
        }
        else if(object->Attribute("type","LAMP"))
        {

                sg::sg_light_object_ptr  obj = sg->add_light_object();
                o = obj.get();
        }
        else if(object->Attribute("type","CAMERA"))
        {

                sg::sg_camera_object_ptr  obj = sg_camera_object_ptr(new sg_camera_object(sg,glm::vec3(0,0,0),glm::vec3(0,0,-1),glm::vec3(0,1,0)));
                o = obj.get();

                sg->add_object(obj);
        }

        o->set_name(object->Attribute("name"));

        if(tinyxml2::XMLElement * pos = object->FirstChildElement("position"))
        {
            o->set_position(glm::vec3(pos->FloatAttribute("x"),pos->FloatAttribute("y"),pos->FloatAttribute("z")));
        }
        else
            throw "nope.avi";

        if(tinyxml2::XMLElement * rot = object->FirstChildElement("rotation"))
        {
            o->set_rotation(glm::quat(glm::vec3(rot->FloatAttribute("x"),rot->FloatAttribute("y"),rot->FloatAttribute("z"))));
        }
        else if(tinyxml2::XMLElement * rot = object->FirstChildElement("quat_rotation"))
        {
            o->set_rotation(glm::quat(rot->FloatAttribute("w"),rot->FloatAttribute("x"),rot->FloatAttribute("y"),rot->FloatAttribute("z")));
        }
        else
            throw "nope.avi";

        if(tinyxml2::XMLElement * scale = object->FirstChildElement("scale"))
        {
            o->set_scale(glm::vec3(scale->FloatAttribute("x"),scale->FloatAttribute("y"),scale->FloatAttribute("z")));
        }
        else
            throw "nope.avi";

        object = object->NextSiblingElement();
    }

    return true;
}

}
