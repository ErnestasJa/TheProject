#include "precomp.h"
#include "iqmesh.h"
iqmesh::iqmesh()
{
}
iqmesh::~iqmesh()
{
}
bool iqmesh::generate()
{
	//set the buffers to invalid values
	for(uint32_t i=0; i<IQM_BUFFER_COUNT; i++)
		buffers[i]=-1;
	//create the VAO
	glGenVertexArrays(1,&vaoid);
	glBindVertexArray(vaoid);
	//keeping track on enabling attrib ids
	uint32_t attribid=0;
	//determine how many buffers we got and check their formats
	for(uint32_t i=0; i<data_header.num_vertexarrays; i++)
	{
		iqmvertexarray va=vertexarrays[i];
		switch(va.type)
		{
			case IQM_POSITION:
			printf("Got a position buffer.\n");
			if(va.format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return false;
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
			if(va.format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return false;
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
			if(va.format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return false;
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
			if(va.format!=IQM_FLOAT)
			{
				printf("Bad format. Cannot continue.\n");
				return false;
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
			if(va.format!=IQM_UBYTE)
			{
				printf("Bad format. Cannot continue.\n");
				return false;
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
			if(va.format!=IQM_UBYTE)
			{
				printf("Bad format. Cannot continue.\n");
				return false;
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
	if(indices.size()>0)
	{
		glGenBuffers(1,&buffers[IQM_INDICES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[IQM_INDICES]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(indices[0]),&indices[0],GL_STATIC_DRAW);
	}
	else
		return false;

//	for(uint32_t i=0; i<attribid; i++)
//		glDisableVertexAttribArray(i);

	glBindVertexArray(0);
	return true;
}

void iqmesh::draw(bool whole)
{
	//bind the meshe's vertex array with its bound buffers
	glBindVertexArray(vaoid);

	if(whole)
	{
		//draw mesh as one unit, saves draw calls.
		glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	}
	else
	{
		//draw each submesh separately, allows more customisation.
		for(uint32_t i=0; i<data_header.num_meshes; i++)
		{
            //glBindTexture(GL_TEXTURE_2D,submeshes[i].mat.texid);
            //draw all sub meshes using index offset
            glDrawElements(GL_TRIANGLES,submeshes[i].num_vertexes*3,GL_UNSIGNED_INT,(void*)(sizeof(uint32_t)*submeshes[i].first_triangle*3));
		}
	}

	//make sure to unbind the vertex array after using it
	glBindVertexArray(0);
}
