#include "iqmloader.h"
#include "iqmesh.h"
iqmesh *iqmloader::load ( const unsigned char* data, const iqmheader &head )
{
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
			printf("Loading %i mesh(es)...\n",head.num_meshes);
		}
		else
		{
			printf("No meshes present. Is it a blank IQM?\n");
			return nullptr;
		}
		
		const char* textstr=(const char*)&data[head.ofs_text];
		iqmmesh* m=(iqmmesh*)&data[head.ofs_meshes];
		for(unsigned int i=0; i<head.num_meshes; i++)
		{
			iqmmesh &temp=m[i];
			printf("TEST INFO:\nName:%s\nMaterial:%s\nF.Vert:%i\nN.Verts:%i\nF.Tri:%i\nN.Tris:%i\n",&textstr[temp.name],&textstr[temp.material],temp.first_vertex,temp.num_vertexes,temp.first_triangle,temp.num_triangles);
		}
	}
	return nullptr;
}
