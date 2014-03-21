#version 330 core
  
layout(location=0) in vec3 vVertex;			 	//per-vertex position
layout(location=2) in vec3 vNormal;  		 	//per-vertex normal

//shader uniforms
uniform mat4 mvp;	//modelview projection matrix
uniform mat4 m;

uniform mat3 n;		//normal matrix
uniform mat4 s;		//shadow matrix

//shader outputs to the fragment shader

smooth out vec3 vEyeSpacePosition;		//eye space position
smooth out vec4 vShadowCoords;			//shadow coordinates

void main()
{ 	
	//multiply the world space vertex position with the shadow matrix 
	//to get the shadow coordinates
	vShadowCoords     = s*(m*vec4(vVertex,1));

	//multiply the combined modelview projection matrix with the object space vertex
	//position to get the clip space position
    gl_Position       = mvp*vec4(vVertex,1); 
}
 
