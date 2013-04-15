#include "precomp.h"
#include "iqmloader.h"
#include "iqmesh.h"

void iqmloader::load_header(const unsigned char* data, iqmheader * & header)
{
    header = (iqmheader*)data[0];
}

iqmesh *iqmloader::load ( const unsigned char* data)
{
    iqmesh *output=nullptr;
    iqmheader * head = nullptr;

    load_header(data,head);


    if ( !strcmp ( head->magic,IQM_MAGIC ) && head->version==IQM_VERSION && head->filesize>0 )
        printf ( "IQM File appears to be correct and not empty. (MAGIC:%s,VERSION:%i)\n",head->magic,head->version );
    else
    {
        printf("IQM file is corrupt or invalid.\n");
        return output;
    }

    output=new iqmesh();

    ///big single line of null terminated >strings<
    output->texts=(const char*)&data[head->ofs_text];

    output->submeshes   =(iqmmesh*)         &data[head->ofs_meshes];
    output->vertexarrays=(iqmvertexarray*)  &data[head->ofs_vertexarrays];
    output->triangles   =(iqmtriangle*)     &data[head->ofs_triangles];
    output->joints      =(iqmjoint*)        &data[head->ofs_joints];
    output->poses       =(iqmpose*)         &data[head->ofs_poses];
    output->anims       =(iqmanim*)         &data[head->ofs_anims];

    //ofs frames? Dunno for now.

    //aparently there's only one bounds entry(A bounding box), hence using [0]
    iqmbounds* bounds=(iqmbounds*)&data[head->ofs_bounds];
    output->bounds.resize(1);
    output->bounds[0]=bounds[0];

    //ofs comments? offset to probably const* char comments

    //ofs extensions? the ones I don't know about

    //put the buffer data in place where it belongs.
    //temporary vecs for filling buffers up.
    glm::vec2 *temp2;
    glm::vec3 *temp3;
    glm::vec4 *temp4;
    glm::detail::tvec4<uint8_t> *tempu4;
    for(uint32_t i=0; i<head->num_vertexarrays; i++)
    {
        iqmvertexarray va=output->vertexarrays[i];
        switch(va.type)
        {
            case IQM_POSITION:
            temp3=(glm::vec3*)&data[va.offset];
            output->positions.resize(head->num_vertexes);

            std::copy(temp3,temp3+head->num_vertexes,&output->positions[0]);

            break;

            case IQM_TEXCOORD:
            temp2=(glm::vec2*)&data[va.offset];
            output->texcoords.resize(head->num_vertexes);

            std::copy(temp2,temp2+head->num_vertexes,&output->texcoords[0]);
            break;

            case IQM_NORMAL:
            temp3=(glm::vec3*)&data[va.offset];
            output->normals.resize(head->num_vertexes);

            std::copy(temp3,temp3+head->num_vertexes,&output->normals[0]);

            break;

            case IQM_TANGENT:
            temp4=(glm::vec4*)&data[va.offset];
            output->tangents.resize(head->num_vertexes);

            std::copy(temp4,temp4+head->num_vertexes,&output->tangents[0]);

            break;

            case IQM_BLENDINDEXES:
            tempu4=(glm::detail::tvec4<uint8_t>*)&data[va.offset];
            output->bindexes.resize(head->num_vertexes);

            std::copy(tempu4,tempu4+head->num_vertexes,&output->bindexes[0]);

            break;

            case IQM_BLENDWEIGHTS:
            tempu4=(glm::detail::tvec4<uint8_t>*)&data[va.offset];
            output->bweights.resize(head->num_vertexes);

            std::copy(tempu4,tempu4+head->num_vertexes,&output->bweights[0]);

            break;


            case IQM_COLOR:
            temp3=(glm::vec3*)&data[va.offset];
            output->colors.resize(head->num_vertexes);

            std::copy(temp3,temp3+head->num_vertexes,&output->colors[0]);

            break;
        }
    }

    //convert triangles to indices and add them.
    output->indices.resize(head->num_triangles*3);
    uint32_t counter=0;
    for(uint32_t i=0; i<head->num_triangles; i++)
    {
        output->indices[counter]=output->triangles[i].verts[0];
        output->indices[counter+1]=output->triangles[i].verts[1];
        output->indices[counter+2]=output->triangles[i].verts[2];
        counter+=3;
    }
    printf("Indices:%i\n",counter);

    //mesh info test
    for(uint32_t i=0; i<head->num_meshes; i++)
    {
        iqsubmesh sm=output->submeshes[i];
        printf("TEST MESH LOADER INFO:\nName:%s\nMaterial:%s\nF.Vert:%i\nN.Verts:%i\nF.Ind:%i\nN.Inds:%i\n",sm.name.c_str(),"NYI",sm.first_vertex,sm.num_vertices,sm.first_index,sm.num_indices);
    }

    if(output->generate())
        return output;
    else
    {
        printf("Mesh generation failed.\n");
        return nullptr;
    }


    return output;
}
