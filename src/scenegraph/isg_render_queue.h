#ifndef ISG_RENDER_QUEUE_H
#define ISG_RENDER_QUEUE_H

namespace sg
{
class isg_object;
class sg_material; typedef std::shared_ptr<sg_material> sg_material_ptr;
class isg_render_queue
{
    public:
        isg_render_queue();
        virtual ~isg_render_queue();

        virtual void add_object(isg_object * obj)=0;
        virtual void set_material(isg_object * obj, sg_material_ptr material)=0;
        virtual void clear()=0;

        virtual void render_all()=0;

    protected:
    private:
};

typedef std::shared_ptr<isg_render_queue> sg_render_queue_ptr;

}

#endif // ISG_RENDER_QUEUE_H
