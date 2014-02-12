#ifndef SG_SPRITE_H
#define SG_SPRITE_H

#include "isg_object.h"

namespace sg
{

class sg_sprite: public isg_object
{
    public:
        sg_sprite(scenegraph * sg);
        virtual ~sg_sprite();

    protected:
    private:
};

}

#endif // SG_SPRITE_H
