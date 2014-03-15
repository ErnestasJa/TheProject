#version 330

  
layout(location=0) in vec3 vVertex;			 	//per-vertex position
layout(location=1) in vec3 vnormal;  		 	//per-vertex normal

//shader uniforms
uniform mat4 mvp;	//modelview projection matrix
uniform mat4 mv;	//modelview matrix
uniform mat4 m;		//model matrix
uniform mat3 n;		//normal matrix
uniform mat4 s;		//shadow matrix

//shader outputs to the fragment shader
smooth out vec3 vEyespacenormal;		//eye space normal
smooth out vec3 vEyespacePosition;		//eye space position
smooth out vec4 vshadowCoords;			//shadow coordinates

void main()
{ 	
	//multiply the object space vertex position with the modelview matrix 
	//to get the eye space vertex position
	vEyespacePosition = (mv*vec4(vVertex,1)).xyz; 

	//multiply the object space normal with the normal matrix 
	//to get the eye space normal
	vEyespacenormal   = n*vnormal;

	//multiply the world space vertex position with the shadow matrix 
	//to get the shadow coordinates
	vshadowCoords     = s*(m*vec4(vVertex,1));

	//multiply the combined modelview projection matrix with the object space vertex
	//position to get the clip space position
    gl_Position       = mvp*vec4(vVertex,1); 
}
