
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

in vec4 vtx_color; //vertex color
in vec3 vtx_pos;
in vec3 vtx_norm;
in vec3 vtx_uv; //vertex UV
in vec4 vtx_tan;

void main()							
{		
	//frag_color = texture(tex_albedo, uv);
	frag_color = texture(tex_albedo, vtx_uv.xy);
}	