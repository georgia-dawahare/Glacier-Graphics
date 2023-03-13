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

out vec4 frag_color;

in vec4 vtx_color; //vertex color
in vec3 vtx_pos;
in vec3 vtx_norm;
in vec3 vtx_uv; //vertex UV
in vec4 vtx_tan;

void main()							
{		
	// Add lighting to all dolphins
	vec3 lightingColor= vec3(0.);
	const vec3 LightPosition = vec3(-1, 1, -9);
	const vec3 LightIntensity = vec3(25);
	const vec3 ka = 0.1 * vec3(1., 1., 1.);
	const vec3 kd = 5 * vec3(1., 1., 1.);
	const vec3 ks = vec3(2.);
	const float n = 400.0;

	vec3 normal = normalize(vtx_norm.xyz);
	vec3 lightDir = LightPosition - vtx_pos;
	float _lightDist = length(lightDir);
	vec3 _lightDir = normalize(lightDir);
	vec3 _localLight = LightIntensity / (_lightDist * _lightDist);
	vec3 ambColor = ka;
	lightingColor = kd * _localLight * max(0., dot(_lightDir, normal));

	frag_color = vec4(lightingColor, 1) * texture(tex_albedo, vtx_uv.xy);
}	