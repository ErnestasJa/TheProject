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
			for(uint i=0; i<head.num_text; i++)
				output->texts.push_back(&texts[i]);
				
			iqmmesh* meshes=(iqmmesh*)&data[head.ofs_meshes];
			for(uint i=0; i<head.num_meshes; i++)
				output->meshes.push_back(&meshes[i]);
				
			iqmvertexarray* vertexarrays=(iqmvertexarray*)&data[head.ofs_vertexarrays];
			for(uint i=0; i<head.num_vertexarrays; i++)
				output->vertexarrays.push_back(&vertexarrays[i]);
			
			iqmtriangle* triangles=(iqmtriangle*)&data[head.ofs_triangles];
			for(uint i=0; i<head.num_triangles; i++)
				output->triangles.push_back(&triangles[i]);
				
			//ofs adjacency?
			
			iqmjoint* joints=(iqmjoint*)&data[head.ofs_joints];
			for(uint i=0; i<head.num_joints; i++)
				output->joints.push_back(&joints[i]);
				
			iqmpose* poses=(iqmpose*)&data[head.ofs_poses];
			for(uint i=0; i<head.num_poses; i++)
				output->poses.push_back(&poses[i]);
				
			iqmanim* anims=(iqmanim*)&data[head.ofs_anims];
			for(uint i=0; i<head.num_anims; i++)
				output->anims.push_back(&anims[i]);
				
			//ofs frames?
			
			iqmbounds* bounds=(iqmbounds*)&data[head.ofs_bounds];
			output->bounds.push_back(&bounds[0]);
			
			//ofs comments?
			
			//ofs extensions?
			
			for(uint8_t i=0; i<head.num_meshes; i++)
			{
				iqmmesh &temp=meshes[i];
				printf("TEST INFO:\nName:%s\nMaterial:%s\nF.Vert:%i\nN.Verts:%i\nF.Tri:%i\nN.Tris:%i\n",&texts[temp.name],&texts[temp.material],temp.first_vertex,temp.num_vertexes,temp.first_triangle,temp.num_triangles);
			}
		}
		else
		{
			printf("No meshes present. Is it a blank IQM?\n");
			return nullptr;
		}
	}
	return output;
}
