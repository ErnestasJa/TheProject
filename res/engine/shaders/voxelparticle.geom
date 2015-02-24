#version 330
// THE FOOD CHAIN GOES LIKE THIS
// Vert
// Geom
// Frag

layout (points) in;
layout (triangle_strip, max_vertices=32) out;

uniform mat4 V;
uniform mat4 P;
 
in vData
{
    vec4 color;
	vec4 pos;
	vec4 rot;
}vertex[];

out vec4 color;
out vec4 pos;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
mat4 rotMat=rotationMatrix(vec3(1,0,0),vertex[0].rot.x);
rotMat=rotMat*rotationMatrix(vec3(0,1,0),vertex[0].rot.y);
rotMat=rotMat*rotationMatrix(vec3(0,0,1),vertex[0].rot.z);

float particleSize=vertex[0].rot.w;

//left
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,-0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,-0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,0.5,0.5,0.0))*rotMat);
EmitVertex();
EndPrimitive();

//right
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,-0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,-0.5,-0.5,0.0))*rotMat);
EmitVertex();
EndPrimitive();

//top
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,0.5,-0.5,0.0))*rotMat);
EmitVertex();
EndPrimitive();

//bottom
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,-0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,-0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,-0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,-0.5,0.5,0.0))*rotMat);
EmitVertex();
EndPrimitive();

//front
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,-0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(0.5,-0.5,0.5,0.0))*rotMat);
EmitVertex();
EndPrimitive();

//back
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,0.5,-0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,-0.5,0.5,0.0))*rotMat);
EmitVertex();
color=vertex[0].color;
gl_Position = P*V*(vertex[0].pos+(particleSize*vec4(-0.5,-0.5,-0.5,0.0))*rotMat);
EmitVertex();
EndPrimitive();
}
