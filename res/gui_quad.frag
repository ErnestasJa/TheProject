#version 330
uniform sampler2D tex;
uniform float alpha;
uniform bool singlechannel;
in vec2 UV;
out vec4 FragColor;
uniform vec3 C;
void main()
{
	if(singlechannel)
	{
		float d=texture(tex,UV).r;
		FragColor=vec4(d,d,d,1);
	}
	else
	{
		FragColor = vec4(texture(tex,UV).rgb,texture(tex,UV).a*alpha);
	}
}
