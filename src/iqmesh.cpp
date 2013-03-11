#include "iqmesh.h"
iqmesh::iqmesh()
{
}
iqmesh::~iqmesh()
{
}
void iqmesh::generate()
{
	//determine how many buffers we got
	for(uint i=0; i<vertexarrays.size(); i++)
	{
		iqmvertexarray *va=vertexarrays[i];
		switch(va->type)
		{
			case IQM_POSITION:
			printf("Got a position buffer.\n");
			if(va->format!=IQM_FLOAT)
				printf("Bad format?\n");
			break;
			
			case IQM_TEXCOORD:
			printf("Got a texcoord buffer.\n");
			break;
			
			case IQM_NORMAL:
			printf("Got a normal buffer.\n");
			break;
			
			case IQM_TANGENT:
			printf("Got a tangent buffer.\n");
			break;
			
			case IQM_BLENDINDEXES:
			printf("Got a blendindex buffer.\n");
			break;
			
			case IQM_BLENDWEIGHTS:
			printf("Got a blendweight buffer.\n");
			break;
			
			case IQM_COLOR:
			printf("Got a color buffer.\n");
			break;
		}
	}
	//create buffers
	//glGenVertexArrays(1,&vaoid);
}
