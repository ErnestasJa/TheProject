#include "precomp.h"
#include "iqmloader.h"
#include "iqmesh.h"
#include "math/util.h"

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
    output->data_buff = (uint8_t*)data;

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

    //aparently there's only one bounds entry(A bounding box), hence using [0]
    output->bounds=(iqmbounds*)&data[head.ofs_bounds];

    for(uint32_t i=0; i<head.num_vertexarrays; i++)
    {
        iqmvertexarray va=output->vertexarrays[i];
        switch(va.type)
        {
        case IQM_POSITION:
            output->positions=(glm::vec3*)&data[va.offset];

            break;

        case IQM_TEXCOORD:
            output->texcoords=(glm::vec2*)&data[va.offset];

            break;

        case IQM_NORMAL:
            output->normals=(glm::vec3*)&data[va.offset];

            break;

        case IQM_TANGENT:
            output->tangents=(glm::vec4*)&data[va.offset];

            break;

        case IQM_BLENDINDEXES:
            output->bindexes=(u8vec4*)&data[va.offset];

            break;

        case IQM_BLENDWEIGHTS:
            output->bweights=(u8vec4*)&data[va.offset];

            break;

        case IQM_COLOR:
            output->colors=(glm::vec3*)&data[va.offset];

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

    return output;
}

bool iqmloader::loadiqmanims(iqmesh * mesh)
{
    iqmheader & hdr = mesh->data_header;
    if(hdr.num_poses != hdr.num_joints) return false;

    mesh->frames = new glm::mat3x4[hdr.num_frames * hdr.num_poses];
    mesh->base_frame = new glm::mat3x4[hdr.num_joints];
    mesh->inverse_base_frame = new glm::mat3x4[hdr.num_joints];
    mesh->current_frame = new glm::mat3x4[hdr.num_joints];

    for(uint32_t i = 0; i < hdr.num_joints; i++)
    {
        iqmjoint &j = mesh->joints[i];
        makeJointMatrix(mesh->base_frame[i],
                        glm::normalize(glm::quat(j.rotate[3],j.rotate[0],j.rotate[1],j.rotate[2])),
                        glm::vec3(j.translate[0],j.translate[1],j.translate[2]),
                        glm::vec3(j.scale[0],j.scale[1],j.scale[2]));

        invert(mesh->inverse_base_frame[i],mesh->base_frame[i]);

        if(j.parent >= 0)
        {
            mesh->base_frame[i] = mul(mesh->base_frame[j.parent], mesh->base_frame[i]);
            mesh->inverse_base_frame[i] = mul(mesh->inverse_base_frame[i],mesh->inverse_base_frame[j.parent]);
        }
    }

    uint16_t * frame_data = (uint16_t *)&mesh->data_buff[hdr.ofs_frames];

    for(int i = 0; i < (int)hdr.num_frames; i++)
    {
        for(int j = 0; j < (int)hdr.num_poses; j++)
        {
            iqmpose &p = mesh->poses[j];
            glm::quat rotate;
            glm::vec3 translate, scale;
            translate.x = p.channeloffset[0];
            if(p.mask&0x01) translate.x += *frame_data++ * p.channelscale[0];
            translate.y = p.channeloffset[1];
            if(p.mask&0x02) translate.y += *frame_data++ * p.channelscale[1];
            translate.z = p.channeloffset[2];
            if(p.mask&0x04) translate.z += *frame_data++ * p.channelscale[2];
            rotate.x = p.channeloffset[3];
            if(p.mask&0x08) rotate.x += *frame_data++ * p.channelscale[3];
            rotate.y = p.channeloffset[4];
            if(p.mask&0x10) rotate.y += *frame_data++ * p.channelscale[4];
            rotate.z = p.channeloffset[5];
            if(p.mask&0x20) rotate.z += *frame_data++ * p.channelscale[5];
            rotate.w = p.channeloffset[6];
            if(p.mask&0x40) rotate.w += *frame_data++ * p.channelscale[6];
            scale.x = p.channeloffset[7];
            if(p.mask&0x80) scale.x += *frame_data++ * p.channelscale[7];
            scale.y = p.channeloffset[8];
            if(p.mask&0x100) scale.y += *frame_data++ * p.channelscale[8];
            scale.z = p.channeloffset[9];
            if(p.mask&0x200) scale.z += *frame_data++ * p.channelscale[9];

            /// Concatenate each pose with the inverse base pose to avoid doing this at animation time.
            /// If the joint has a parent, then it needs to be pre-concatenated with its parent's base pose.
            /// Thus it all negates at animation time like so:
            ///   (parentPose * parentInverseBasePose) * (parentBasePose * childPose * childInverseBasePose) =>
            ///   parentPose * (parentInverseBasePose * parentBasePose) * childPose * childInverseBasePose =>
            ///   parentPose * childPose * childInverseBasePose

            glm::mat3x4 m;
            makeJointMatrix(m, glm::normalize(rotate), translate, scale);

            if(p.parent >= 0) mesh->frames[i*hdr.num_poses + j] = mul(mul(mesh->base_frame[p.parent],m), mesh->inverse_base_frame[j]);
            else mesh->frames[i*hdr.num_poses + j] = mul(m, mesh->inverse_base_frame[j]);
        }
    }

    for(int i = 0; i < (int)hdr.num_anims; i++)
    {
        iqmanim &a = mesh->anims[i];
        printf("Loaded anim: %s\n", mesh->texts[a.name]);
    }

    return true;
}
