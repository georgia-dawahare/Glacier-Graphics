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
uniform float iTime;					// time
uniform sampler2D tex_albedo;			// texture color
uniform sampler2D tex_normal;			// texture normal
uniform float M_PI = 3.14159265;

/*input variables*/
in vec4 vtx_color; 
in vec3 vtx_pos;
in vec3 vtx_norm;
in vec3 vtx_uv; 
in vec4 vtx_tan;

/*output variables*/
out vec4 frag_color;


void main()							
{		
	float rad = 10.0;

	float u = (atan(vtx_pos.x, vtx_pos.z)/(2*M_PI)+0.5);
	float v = (0.5 - asin(vtx_pos.y / rad)/(M_PI));
	vec2 uv = vec2(u, v);
	
	frag_color = texture(tex_albedo, uv);
}	