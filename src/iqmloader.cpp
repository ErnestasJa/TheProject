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
			for(uint i=0; i<head.num_text; i++)
				output->texts[i]=&texts[i];
				
			iqmmesh* meshes=(iqmmesh*)&data[head.ofs_meshes];
			output->meshes.resize(head.num_meshes);
			for(uint i=0; i<head.num_meshes; i++)
				output->meshes[i]=&meshes[i];
				
			iqmvertexarray* vertexarrays=(iqmvertexarray*)&data[head.ofs_vertexarrays];
			output->vertexarrays.resize(head.num_vertexarrays);
			for(uint i=0; i<head.num_vertexarrays; i++)
				output->vertexarrays[i]=&vertexarrays[i];
			
			iqmtriangle* triangles=(iqmtriangle*)&data[head.ofs_triangles];
			output->triangles.resize(head.num_triangles);
			for(uint i=0; i<head.num_triangles; i++)
				output->triangles[i]=&triangles[i];
				
			//ofs adjacency? Dunno what to do with this. Adjacent triangles, duh.
			
			iqmjoint* joints=(iqmjoint*)&data[head.ofs_joints];
			output->joints.resize(head.num_joints);
			for(uint i=0; i<head.num_joints; i++)
				output->joints[i]=&joints[i];
				
			iqmpose* poses=(iqmpose*)&data[head.ofs_poses];
			output->poses.resize(head.num_poses);
			for(uint i=0; i<head.num_poses; i++)
				output->poses[i]=&poses[i];
				
			iqmanim* anims=(iqmanim*)&data[head.ofs_anims];
			output->anims.resize(head.num_anims);
			for(uint i=0; i<head.num_anims; i++)
				output->anims[i]=&anims[i];
				
			//ofs frames? Dunno for now.
			
			//aparently there's only one bounds entry(A bounding box), hence using [0]
			iqmbounds* bounds=(iqmbounds*)&data[head.ofs_bounds];
			output->bounds.resize(1);
			output->bounds[0]=&bounds[0];
			
			//ofs comments? offset to probably const* char comments
			
			//ofs extensions? the ones I don't know about
			
			//a test
			for(uint8_t i=0; i<head.num_meshes; i++)
			{
				iqmmesh &temp=meshes[i];
				printf("TEST MESH LOADER INFO:\nName:%s\nMaterial:%s\nF.Vert:%i\nN.Verts:%i\nF.Tri:%i\nN.Tris:%i\n",&texts[temp.name],&texts[temp.material],temp.first_vertex,temp.num_vertexes,temp.first_triangle,temp.num_triangles);
			}
		}
		else
		{
			printf("No meshes present. Is it a blank IQM?\n");
			return nullptr;
		}
		return output;
	}
	else
		return nullptr;
}
