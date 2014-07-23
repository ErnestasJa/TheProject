#version 330

uniform sampler2D texture0;

in vec3 _normal;
in vec2 _uv;

const vec3 light_dir = vec3(1,10,1);

void main(){
        vec3 N = normalize(_normal);

		vec3 tex_diffuse = texture(texture0, _uv).rgb;
		//vec3 diffuse = tex_diffuse * max(dot(N,light_dir), 0.0);
			
		//gl_FragColor = vec4(1,0,0,1);
		gl_FragColor = vec4(tex_diffuse,1);
}