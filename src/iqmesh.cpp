#include "iqmesh.h"
iqmesh::iqmesh()
{
}
iqmesh::~iqmesh()
{
}
void iqmesh::load ( const unsigned char *data,const iqmheader &head )
{
	if ( head!=nullptr )
	{
		if ( !strcmp ( head.magic,IQM_MAGIC ) )
			printf ( "Magic is correct.\n" );
		if ( head.version==IQM_VERSION )
			printf ( "IQM Version is correct. (IQM_VERSION=%i)\n",head.version );
	}
}
