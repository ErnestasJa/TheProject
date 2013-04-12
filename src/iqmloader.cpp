#include "precomp.h"
#include "iqmloader.h"
#include "iqmesh.h"

iqmesh *iqmloader::load ( const unsigned char* data, const iqmheader &head )
{
	iqmesh *output=nullptr;
	if ( sizeof ( head ) >0 )
	{
		if ( !strcmp ( head.magic,IQM_MAGIC ) && head.version==IQM_VERSION && head.filesize>0 )
			printf ( "IQM File appears to be correct and not empty. (MAGIC:%s,VERSION:%i)\n",head.magic,head.version );
		else
		{
			printf("IQM file is corrupt or invalid.\n");
			return nullptr;
		}
		if(head.num_meshes>0)
		{
			output=new iqmesh();

			printf("Loading %i mesh(es)...\n",head.num_meshes);

			const char* texts=(const char*)&data[head.ofs_text];
			output->texts.resize(head.num_text);
			for(uint32_t i=0; i<head.num_text; i++)
				output->texts[i]=&texts[i];

			iqmmesh* meshes=(iqmmesh*)&data[head.ofs_meshes];
			output->submeshes.resize(head.num_meshes);
			for(uint32_t i=0; i<head.num_meshes; i++)
			{
				iqmmesh *temp=&meshes[i];
				iqsubmesh sub;

				sub.name=std::string(output->texts[temp->name]);
				sub.first_index=temp->first_triangle*3u;
				sub.num_indices=temp->num_triangles*3u;
				sub.first_vertex=temp->first_vertex;
				sub.num_vertices=temp->num_vertexes;
				sub.visible=true;

				output->submeshes[i]=sub;
			}

			iqmvertexarray* vertexarrays=(iqmvertexarray*)&data[head.ofs_vertexarrays];
			output->vertexarrays.resize(head.num_vertexarrays);
			for(uint32_t i=0; i<head.num_vertexarrays; i++)
				output->vertexarrays[i]=vertexarrays[i];

			iqmtriangle* triangles=(iqmtriangle*)&data[head.ofs_triangles];
			output->triangles.resize(head.num_triangles);
			for(uint32_t i=0; i<head.num_triangles; i++)
				output->triangles[i]=triangles[i];

			//ofs adjacency? Dunno what to do with this. Adjacent triangles, duh.

			iqmjoint* joints=(iqmjoint*)&data[head.ofs_joints];
			output->joints.resize(head.num_joints);
			for(uint32_t i=0; i<head.num_joints; i++)
				output->joints[i]=joints[i];

			iqmpose* poses=(iqmpose*)&data[head.ofs_poses];
			output->poses.resize(head.num_poses);
			for(uint32_t i=0; i<head.num_poses; i++)
				output->poses[i]=poses[i];

			iqmanim* anims=(iqmanim*)&data[head.ofs_anims];
			output->anims.resize(head.num_anims);
			for(uint32_t i=0; i<head.num_anims; i++)
				output->anims[i]=anims[i];

			//ofs frames? Dunno for now.

			//aparently there's only one bounds entry(A bounding box), hence using [0]
			iqmbounds* bounds=(iqmbounds*)&data[head.ofs_bounds];
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
			for(uint32_t i=0; i<head.num_vertexarrays; i++)
			{
				iqmvertexarray va=output->vertexarrays[i];
				switch(va.type)
				{
					case IQM_POSITION:
					temp3=(glm::vec3*)&data[va.offset];
					output->positions.resize(head.num_vertexes);

					std::copy(temp3,temp3+head.num_vertexes,&output->positions[0]);

					break;

					case IQM_TEXCOORD:
					temp2=(glm::vec2*)&data[va.offset];
					output->texcoords.resize(head.num_vertexes);

					std::copy(temp2,temp2+head.num_vertexes,&output->texcoords[0]);
					break;

					case IQM_NORMAL:
					temp3=(glm::vec3*)&data[va.offset];
					output->normals.resize(head.num_vertexes);

					std::copy(temp3,temp3+head.num_vertexes,&output->normals[0]);

					break;

					case IQM_TANGENT:
					temp4=(glm::vec4*)&data[va.offset];
					output->tangents.resize(head.num_vertexes);

					std::copy(temp4,temp4+head.num_vertexes,&output->tangents[0]);

					break;

					case IQM_BLENDINDEXES:
					tempu4=(glm::detail::tvec4<uint8_t>*)&data[va.offset];
					output->bindexes.resize(head.num_vertexes);

					std::copy(tempu4,tempu4+head.num_vertexes,&output->bindexes[0]);

					break;

					case IQM_BLENDWEIGHTS:
					tempu4=(glm::detail::tvec4<uint8_t>*)&data[va.offset];
					output->bweights.resize(head.num_vertexes);

					std::copy(tempu4,tempu4+head.num_vertexes,&output->bweights[0]);

					break;


					case IQM_COLOR:
					temp3=(glm::vec3*)&data[va.offset];
					output->colors.resize(head.num_vertexes);

					std::copy(temp3,temp3+head.num_vertexes,&output->colors[0]);

					break;
				}
			}

			//convert triangles to indices and add them.
			output->indices.resize(head.num_triangles*3);
			uint32_t counter=0;
			for(uint32_t i=0; i<head.num_triangles; i++)
			{
				output->indices[counter]=output->triangles[i].verts[0];
				output->indices[counter+1]=output->triangles[i].verts[1];
				output->indices[counter+2]=output->triangles[i].verts[2];
				counter+=3;
			}
			printf("Indices:%i\n",counter);

			//mesh info test
			for(uint32_t i=0; i<head.num_meshes; i++)
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
		}
		else
		{
			printf("No meshes present. Is it a blank IQM?\n");
			return nullptr;
		}
	}
	else
		return nullptr;
}
