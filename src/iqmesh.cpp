#include "iqmesh.h"
iqmesh::iqmesh()
{
}
iqmesh::~iqmesh()
{
}
void iqmesh::generate()
{
	//set the buffers to invalid values
	for(uint i=0; i<IQM_BUFFER_COUNT; i++)
		buffers[i]=-1;
	//create the VAO
	glGenVertexArrays(1,&vaoid);
	glBindVertexArray(vaoid);
	//keeping track on enabling attrib ids
	uint attribid=0;
	//determine how many buffers we got and check their formats
	for(uint i=0; i<vertexarrays.size(); i++)
	{
		iqmvertexarray *va=vertexarrays[i];
		switch(va->type)
		{
			case IQM_POSITION:
			printf("Got a position buffer.\n");
			if(va->format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return;
			}
			glGenBuffers(1,&buffers[IQM_POSITION]);
			glBindBuffer(GL_ARRAY_BUFFER,buffers[IQM_POSITION]);
			glBufferData(GL_ARRAY_BUFFER,positions.size()*sizeof(positions[0]),&positions[0],GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribid);
			glVertexAttribPointer(attribid,3,GL_FLOAT,GL_FALSE,0,0);
			attribid++;
			break;
			
			case IQM_TEXCOORD:
			printf("Got a texcoord buffer.\n");
			if(va->format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return;
			}
			glGenBuffers(1,&buffers[IQM_TEXCOORD]);
			glBindBuffer(GL_ARRAY_BUFFER,buffers[IQM_TEXCOORD]);
			glBufferData(GL_ARRAY_BUFFER,texcoords.size()*sizeof(texcoords[0]),&texcoords[0],GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribid);
			glVertexAttribPointer(attribid,2,GL_FLOAT,GL_FALSE,0,0);
			attribid++;
			break;
			
			case IQM_NORMAL:
			printf("Got a normal buffer.\n");
			if(va->format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return;
			}
			glGenBuffers(1,&buffers[IQM_NORMAL]);
			glBindBuffer(GL_ARRAY_BUFFER,buffers[IQM_NORMAL]);
			glBufferData(GL_ARRAY_BUFFER,normals.size()*sizeof(normals[0]),&normals[0],GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribid);
			glVertexAttribPointer(attribid,3,GL_FLOAT,GL_FALSE,0,0);
			attribid++;
			break;
			
			case IQM_TANGENT:
			printf("Got a tangent buffer.\n");
			if(va->format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return;
			}
			glGenBuffers(1,&buffers[IQM_TANGENT]);
			glBindBuffer(GL_ARRAY_BUFFER,buffers[IQM_TANGENT]);
			glBufferData(GL_ARRAY_BUFFER,tangents.size()*sizeof(tangents[0]),&tangents[0],GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribid);
			glVertexAttribPointer(attribid,4,GL_FLOAT,GL_FALSE,0,0);
			attribid++;
			break;
			
			case IQM_BLENDINDEXES:
			printf("Got a blendindex buffer.\n");
			if(va->format!=IQM_UBYTE)
			{
				printf("Bad format. Cannot continue.\n");
				return;
			}
			glGenBuffers(1,&buffers[IQM_BLENDINDEXES]);
			glBindBuffer(GL_ARRAY_BUFFER,buffers[IQM_BLENDINDEXES]);
			glBufferData(GL_ARRAY_BUFFER,bindexes.size()*sizeof(bindexes[0]),&bindexes[0],GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribid);
			glVertexAttribPointer(attribid,4,GL_UNSIGNED_SHORT,GL_FALSE,0,0);
			attribid++;
			break;
			
			case IQM_BLENDWEIGHTS:
			printf("Got a blendweight buffer.\n");
			if(va->format!=IQM_UBYTE)
			{
				printf("Bad format. Cannot continue.\n");
				return;
			}
			glGenBuffers(1,&buffers[IQM_BLENDWEIGHTS]);
			glBindBuffer(GL_ARRAY_BUFFER,buffers[IQM_BLENDWEIGHTS]);
			glBufferData(GL_ARRAY_BUFFER,bweights.size()*sizeof(bweights[0]),&bweights[0],GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribid);
			glVertexAttribPointer(attribid,4,GL_UNSIGNED_SHORT,GL_FALSE,0,0);
			attribid++;
			break;
			
			//whoever uses that.. :D
			case IQM_COLOR:
			printf("Got a color buffer. Lol.\n");
			break;
		}
	}
	glGenBuffers(1,&buffers[IQM_INDICES]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[IQM_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);
	
//	for(uint i=0; i<attribid; i++)
//		glDisableVertexAttribArray(i);
	
	glBindVertexArray(0);
}

void iqmesh::draw()
{
	glBindVertexArray(vaoid);
	for(uint i=0; i<meshes.size(); i++)
		glDrawElementsBaseVertex(GL_TRIANGLES,meshes[i]->num_triangles*3,GL_UNSIGNED_INT,(void*)(sizeof(uint) * (meshes[i]->first_triangle*3)),meshes[i]->first_vertex);
	glBindVertexArray(0);
}
