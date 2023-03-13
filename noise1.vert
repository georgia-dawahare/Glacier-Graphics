#version 330 core
#define E 2.71828182845904523536;

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);
	
	rand  = 20. * 1.05 * fract(v * 2.3183099 + vec2(0.71, 0.113));
    rand = -2.0 + 2.5 * 2. * fract(rand.x * rand.y * (rand.x + rand.y) * rand);
	return rand;
}

float perlin_noise(vec2 v) 
{
	float noise = 0;

    vec2 i = floor(v);
    vec2 f = fract(v);
	vec2 u = f * f * (3.0 - 2.0 * f);

	vec2 a = i;
	vec2 b = i + vec2(1.0, 0.0);
	vec2 c = i + vec2(0.0, 1.0);
	vec2 d = i + vec2(1.0, 1.0);

    noise = mix(mix(dot(hash2(a), f), 
        	dot(hash2(b), f - vec2(1.0, 0.0)), u.x),
        	mix(dot(hash2(c), f - vec2(0.0, 1.0)), 
            dot(hash2(d), f - vec2(1.0, 1.0)), u.x), u.y);
	return noise;
}

float noiseOctave(vec2 v, int num)
{
	float sum = 0;

	for (int i = 0; i < num; i++) {
		sum += pow(2.0, -i) * perlin_noise(pow(2.0, i) * v);
	}
	return sum;
}

float height(vec2 v){
    float h = 0;
	float h1 = 0.75 * noiseOctave(v, 10);
	float h2 = 0.5 * pow(2.71828182845904523536, noiseOctave(v, 10));
	h = h1 + h2;
	return h;
}

/*uniform variables*/
uniform float iTime;				

/*input variables*/
layout (location=0) in vec4 pos;
layout (location=2) in vec4 normal;
layout (location=3) in vec4 uv;
layout (location=4) in vec4 tangent;

/*output variables*/
out vec3 vtx_pos;		// vertex position in the world space

void main()												
{
	vtx_pos = vec3(pos.x, height(pos.xz), pos.z);
	gl_Position = pvm * vec4(vtx_pos.xyz, 1.);
}