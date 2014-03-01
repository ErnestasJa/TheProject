#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform vec3 cam_right;
uniform vec3 cam_up;
uniform mat4 vp; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform vec3 bpos; // Position of the center of the billboard
uniform vec3 size; // Size of the billboard, in world units (probably meters)

void main()
{
	vec3 particleCenter_wordspace = bpos;
	
	vec3 vertexPosition_worldspace =  particleCenter_wordspace + cam_right * pos.x * size.x + cam_up * pos.y * size.y;

	// Output position of the vertex
	gl_Position = vp * vec4(vertexPosition_worldspace, 1.0f);

	UV = -pos.xy + vec2(0.5, 0.5);
}