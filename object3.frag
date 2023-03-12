/*This is your fragment shader for texture and normal mapping*/

#version 330 core

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*uniform variables*/
uniform float iTime;					////time
uniform sampler2D tex_albedo;			////texture color
uniform sampler2D tex_normal;			////texture normal

out vec4 frag_color;
in vec3 vtx_uv;

void main()							
{		
	// vec3 uv = vtx_uv;
	// uv.y = uv.y + 0.1 * sin(iTime + 10.0 * uv.x);
	// vec4 textureColor = texture(tex_albedo, uv.xy);
	// frag_color = textureColor;
	float time = iTime;
  	vec2 uv = vtx_uv.xy;

  	vec2 center = vec2(0.0);
  	vec2 coord = uv;
  	vec2 centered_coord = 2.0*uv-1.0;

  	float shutter = 0.9;
  	float texelDistance = distance(center, centered_coord);
  	float dist = 1.41*1.41*shutter - texelDistance;

  	float ripples = 1.0- sin(texelDistance*32.0 - 2.0*time);
  	coord -= normalize(centered_coord-center)*clamp(ripples,0.0,1.0)*0.050;
    
  	vec4 color = texture(tex_albedo, coord);
  	frag_color = vec4(color*dist);

}	
