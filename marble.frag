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

const float PI = 3.14;

out vec4 frag_color;

in vec4 vtx_color; //vertex color
in vec3 vtx_pos;
in vec3 vtx_norm;
in vec3 vtx_uv; //vertex UV
in vec4 vtx_tan;

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(dot(v,vec2(127.1,311.7)),dot(v,vec2(269.5,183.3))); // from the book of shaders
	rand = -1.0 + 2.0*fract(sin(rand)*43758.5453123);

	return rand;
}

float perlin_noise(vec2 v) 
{
	float noise = 0;

	vec2 i = floor(v); // integer part
    vec2 f = fract(v); // fractional part

	vec2 m = smoothstep(0.,1.,f);

    noise = mix(mix(dot( hash2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                    dot( hash2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), m.x),
                mix(dot( hash2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                    dot( hash2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), m.x), m.y);

	return noise;
}

// Generate marble pattern
float t(vec2 uv )
{
    float
    value = perlin_noise(uv / 64.) * 10000.;
    value += perlin_noise(uv / 32.) * 10000.;
    value += perlin_noise(uv / 16.) * 10000.;
    value += perlin_noise(uv / 8.) * 8.;
    value += perlin_noise(uv / 4.) * 4.;
    value += perlin_noise(uv / 2.) * 2.;
    value += perlin_noise(uv);
    value += perlin_noise(uv / .5) * .5;
    value += perlin_noise(uv / .25) * .25;
    return value;
}

void main()							
{		
    vec2 uv2 = vtx_pos.xy*.1;
    float v=0.5+0.5*sin(uv2.x+uv2.y+t(uv2));
	frag_color = vec4(v+.01,v,v,1.0);
}	