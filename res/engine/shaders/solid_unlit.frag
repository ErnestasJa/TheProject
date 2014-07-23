#version 330

in vec3 _normal;

void main(){
        vec3 N = normalize(_normal); 
        gl_FragColor = vec4((N + vec3(1.0, 1.0, 1.0)) / 2.0,1.0);
}