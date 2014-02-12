#version 330 core

smooth out vec4 vSmoothColor;	//output to fragment shader

//shader uniforms
uniform mat4 MVP;

void main()
{
	//local variables
	vec3 pos=vec3(0);

	//get the spawn time by multiplying the current vertex ID with
	//the rate of emission
	float t = gl_VertexID*rate;
	//start with alpha of 1 (fully visible)
	float alpha = 1;
	
	if(time>t) {
		float dt = mod((time-t), life);
		vec2 xy = vec2(gl_VertexID,t); 
		vec2 rdm=vec2(0);
		//point emitter source
		//pos = ((uniformRadomDir(xy) + 0.5*(a+g)*dt)*dt); //for adding gravity 	   
	   
		pos = ((uniformRadomDir(xy, rdm) + 0.5*a*dt)*dt);       //for fire effect from a point emitter
	   
		/*
	    pos = ( uniformRadomDir(xy, rdm) + 0.5*a*dt)*dt;       //for fire effect from a quad emitter
	    vec2 rect = (rdm*2.0 - 1.0) ;
	    pos += vec3(rect.x, 0, rect.y) ;
	    */
	    
	    /*
	    pos = ( uniformRadomDir(xy, rdm) + 0.5*a*dt)*dt;       //for fire effect from a disc emitter
	    vec2 rect = (rdm*2.0 - 1.0);
	    float dotP = dot(rect, rect);
	    
	    if(dotP<1)
	        pos += vec3(rect.x, 0, rect.y) ;
	    */
	    
		alpha = 1.0 - (dt/life);	  
	}
   
	//linearly interpolate between red and yellow colour
	vSmoothColor = vec4(mix(RED,YELLOW,alpha),alpha);
	//get clipspace position
	gl_Position = MVP*vec4(pos,1);
}
