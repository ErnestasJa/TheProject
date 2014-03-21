#version 330 core

layout(location=0) out vec4 vFragColor;	//fragment shader output

//shader uniforms
uniform sampler2D texture0;
const vec3 diffuse_color = vec3(0.8,0.8,0.8);
									 
smooth in vec4 vShadowCoords;		//interpolated shadow coordinates

void main() {   

	float diffuse = 1;

	  
	//if the homogeneous coordinate is > 1, we are in the forward half
	//so we should cast shadows. If this check is removed, you will see 
	//shadows on both sides of the light when the light is very close to 
	//the plane. Try removing this to see what I mean.
	if(vShadowCoords.w>1) {
		
		//divide the shadow coordinate by homogeneous coordinate
		vec3 uv = vShadowCoords.xyz/vShadowCoords.w;
		
		//get the depth value
		float depth = uv.z;
		
		//read the moments from the shadow map texture
		vec4 moments = texture(texture0, uv.xy); 

		//calculate variance from the moments
		float E_x2 = moments.y;
		float Ex_2 = moments.x*moments.x;
		float var = E_x2-Ex_2;

		//bias the variance
		var = max(var, 0.000002);

		//subtract the fragment depth from the  first moment
		//divide variance by the squared difference value
		//to get the maximum probability of fragment to be in shadow
		float mD = depth-moments.x;
		float mD_2 = mD*mD; 
		float p_max = var/(var+ mD_2); 

		//darken the diffuse component if the current depth is less than or equal
		//to the first moment and the retured value is less than the calculated
		//maximum probability
		diffuse *= max(p_max, (depth<=moments.x)?1.0:0.2); 
	}
	//return the final colour by multiplying the diffuse colour with the diffuse component
	vFragColor = diffuse*vec4(diffuse_color, 1);	 
}
