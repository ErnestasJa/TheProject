#include "precomp.h"
#include "iqmloader.h"
#include "iqmesh.h"

void iqmloader::load_header(const char* data, iqmheader & header)
{
    memcpy((void*)&header,(void*)data,sizeof(header));
}

iqmesh *iqmloader::load ( const char* data)
{
    iqmesh *output=nullptr;
    iqmheader head;

    load_header(data,head);


    if ( !strcmp ( IQM_MAGIC, head.magic) && head.version==IQM_VERSION && head.filesize>0 )
        printf ( "IQM File appears to be correct and not empty. (MAGIC:%s,VERSION:%i)\n",head.magic,head.version );
    else
    {
        printf("IQM file is corrupt or invalid.\n");
        return output;
    }

    output=new iqmesh();

    ///big single line of null terminated >strings<
    const char* texts=(const char*)&data[head.ofs_text];

    output->texts = new const char * [head.num_text];

    for(uint32_t i=0; i<head.num_text; i++)
        output->texts[i]=&texts[i];

    output->submeshes   =(iqmmesh*)         &data[head.ofs_meshes];
    output->vertexarrays=(iqmvertexarray*)  &data[head.ofs_vertexarrays];
    output->triangles   =(iqmtriangle*)     &data[head.ofs_triangles];
    output->joints      =(iqmjoint*)        &data[head.ofs_joints];
    output->poses       =(iqmpose*)         &data[head.ofs_poses];
    output->anims       =(iqmanim*)         &data[head.ofs_anims];

    //ofs frames? Dunno for now.

    //aparently there's only one bounds entry(A bounding box), hence using [0]
    output->bounds=(iqmbounds*)&data[head.ofs_bounds];

    //put the buffer data in place where it belongs.
    //temporary vecs for filling buffers up.
    glm::vec2 *temp2;
    glm::vec3 *temp3;
    glm::vec4 *temp4;
    glm::detail::tvec4<uint8_t> *tempu4;
    for(uint32_t i=0; i<head.num_vertexarrays; i++)
    {
        iqmvertexarray va=output->vertexarrays[i];
        switch(va.type)
        {
        case IQM_POSITION:
            output->positions=(vec3*)&data[va.offset];

            break;

        case IQM_TEXCOORD:
            output->texcoords=(vec2*)&data[va.offset];

            break;

        case IQM_NORMAL:
            output->normals=(vec3*)&data[va.offset];

            break;

        case IQM_TANGENT:
            output->tangents=(vec4*)&data[va.offset];

            break;

        case IQM_BLENDINDEXES:
            output->bindexes=(u8vec4*)&data[va.offset];

            break;

        case IQM_BLENDWEIGHTS:
            output->bweights=(u8vec4*)&data[va.offset];

            break;

        case IQM_COLOR:
            output->colors=(vec3*)&data[va.offset];

            break;
        }
    }

    //mesh info test
    for(uint32_t i=0; i<head.num_meshes; i++)
    {
        iqmmesh sm=output->submeshes[i];
        printf("TEST MESH LOADER INFO:\nName:%i\nMaterial:%s\nF.Vert:%i\nN.Verts:%i\nF.Ind:%i\nN.Inds:%i\n",sm.name,"NYI",sm.first_vertex,sm.num_vertexes,sm.first_triangle*3,sm.num_triangles*3);
    }

    output->data_header = head;

    if(output->generate())
        return output;
    else
    {
        printf("Mesh generation failed.\n");
        return nullptr;
    }


    return output;
}
