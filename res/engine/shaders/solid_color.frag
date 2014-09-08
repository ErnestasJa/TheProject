#version 330

in vec3 _col;

void main(){
        gl_FragColor = vec4(_col,1);//vec4(1,0,0,1);
}