#include "precomp.h"
#include "sg_scenegraph_loader.h"
#include "scenegraph.h"
#include "sg_empty_object.h"
#include "../tests/game/app_context.h"

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

bool sg_scenegraph_loader::load_scene(app_context * app_ctx, const std::string & filename,bool with_physics)
{
    std::string scene_path = filename.substr(0,filename.rfind("/")+1);
    std::cout << "scene_path =" << scene_path.c_str() << std::endl;

    tinyxml2::XMLDocument doc;
    uint32_t len;
    char * buf;
    len = helpers::read(filename, buf);

    if(len==0)
        return false;

    doc.Parse(buf);

    tinyxml2::XMLElement * scene = doc.FirstChildElement("scene");
    tinyxml2::XMLElement * object = scene->FirstChildElement("object");

    while(object)
    {
        isg_object * o = nullptr;
        bool mesh_obj=false;

        if(object->Attribute("type","MESH"))
        {
            mesh_obj=true;
            if(tinyxml2::XMLElement * file = object->FirstChildElement("iqm_file"))
            {
                std::cout <<"Got object [" << object->Attribute("name") << "], file = " << file->Attribute("name") << std::endl;

                sg::sg_mesh_object_ptr  obj = app_ctx->sg->load_mesh_object(scene_path+"/"+file->Attribute("name"),true);
                o = obj.get();

                app_ctx->sg->add_object(obj);
            }
        }
        else if(object->Attribute("type","LAMP"))
        {

            sg::sg_light_object_ptr  obj = app_ctx->sg->add_light_object();
            o = obj.get();
        }
        else if(object->Attribute("type","CAMERA"))
        {
            sg::sg_camera_object_ptr  obj = sg_camera_object_ptr(new sg_camera_object(app_ctx->sg,glm::vec3(0,0,0),glm::vec3(0,0,-1),glm::vec3(0,1,0)));
            o = obj.get();

            app_ctx->sg->add_object(obj);
        }
        else if(object->Attribute("type","EMPTY"))
        {
            printf("ADDED EMPTY OBJ: %s\n",object->Attribute("name"));
            sg::sg_empty_object_ptr obj=share(new sg::sg_empty_object(app_ctx->sg));
            app_ctx->sg->add_trigger(obj,object->Attribute("name"));
            o = obj.get();
        }
        else
        {
            object = object->NextSiblingElement();
            continue;
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

        if(with_physics&&mesh_obj)
        {
            sg_mesh_object_ptr mo=app_ctx->sg->get_mesh_obj(o->get_name());
            if(app_ctx->pm->create_trimesh_body(mo,physics_manager::glm_to_bt(o->get_scale()))==nullptr)
                exit(-13337);
        }

        object = object->NextSiblingElement();
    }

    return true;
}

}
