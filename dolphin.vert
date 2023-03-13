/*This is your first vertex shader!*/

#version 330 core

#define PI 3.14159265

/*default camera matrices. do not modify.*/
layout (std140) uniform camera
{
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

/*input variables*/


layout (location=0) in vec4 pos;		/*vertex position*/
layout (location=1) in vec4 color;		/*vertex color*/
layout (location=2) in vec4 normal;		/*vertex normal*/	
layout (location=3) in vec4 uv;			/*vertex uv*/		
layout (location=4) in vec4 tangent;	/*vertex tangent*/

/*output variables*/
out vec4 vtx_color;
out vec3 vtx_pos;
out vec3 vtx_norm;
out vec3 vtx_uv; 
out vec4 vtx_tan; 

void main()												
{
	/*camera-transformed position. do not modify.*/
	gl_Position=pvm*vec4(pos.xyz,1.f);

	vtx_color=vec4(color.rgb,1.f);
	vtx_pos=vec3(pos.xyz);
	vtx_norm=vec3(normal.xyz);
	vtx_uv=vec3(uv.xyz);
	vtx_tan=vec4(tangent);
}	