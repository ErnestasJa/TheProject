#version 330
uniform sampler2D tex;
uniform float alpha;
uniform bool singlechannel=false;
uniform bool coloured=false;
uniform vec4 color;
in vec2 UV;
out vec4 FragColor;
void main()
{
	if(singlechannel)
	{
		float d=texture(tex,UV).r;
		FragColor=vec4(d,d,d,1);
	}
	else if(coloured)
	{
		FragColor=color;
	}
	else
	{
		FragColor = vec4(texture(tex,UV).rgb,texture(tex,UV).a*alpha);
	}
}
