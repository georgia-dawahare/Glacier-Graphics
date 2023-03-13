#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

/*uniform variables*/
uniform float iTime;				

/*input variables*/
in vec3 vtx_pos;

/*input variables*/
out vec4 frag_color;

vec2 hash2(vec2 v)
{
	vec2 rand = vec2(0,0);

	rand  = 30.0 * 1.05 * fract(v * 0.0183099 + vec2(0.11, 0.3));
    rand = -0.4 + 2 * 1.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);
	return rand;
}

float perlin_noise(vec2 v) 
{
	float noise = 0;

    vec2 i = floor(v);
    vec2 f = fract(v);
	vec2 u = f * f * (3.0 - 2.0 * f);

	// Four corners in 2D of a tile
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
	float h1 = 0.02 * noiseOctave(v, 3);
	float h2 = 0.5 * pow(2.71828182845904523536, noiseOctave(v, 15));	// jagged
	h = h1 + h2;
	return h;
}

vec3 compute_normal(vec2 v, float d)
{	
	vec3 normal_vector = vec3(0,0,0);
	vec3 v1 = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y)));
	vec3 v2 = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y)));
	vec3 v3 = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d)));
	vec3 v4 = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d)));
	normal_vector = normalize(cross(v1 - v2, v3 - v4));
	return normal_vector;
}

vec3 get_color(vec2 v)
{
	vec3 vtx_normal = compute_normal(v, 0.01);
	vec3 dark_blue = vec3(32, 37, 71) / 255.;
	vec3 blue = vec3(65,119,198) / 255.;
	vec3 purple = vec3(139,150,199) / 255.;
	vec3 emissiveColor = vec3(0.);
	vec3 lightingColor= vec3(0.);

	const vec3 LightPosition = vec3(0, 1, 3.5);
	const vec3 LightIntensity = vec3(19);
	const vec3 ka = 15 * vec3(1., 1., 1.);
	const vec3 kd = 0.4 * vec3(1., 1., 1);
	const vec3 ks = vec3(2.);
	const float n = 400.0;

	vec3 normal = normalize(vtx_normal.xyz);
	vec3 lightDir = LightPosition - vtx_pos;
	float _lightDist = length(lightDir);
	vec3 _lightDir = normalize(lightDir);
	vec3 _localLight = LightIntensity / (_lightDist * _lightDist);
	vec3 ambColor = ka;
	lightingColor = ka * kd * _localLight * max(0., dot(_lightDir, normal));

	float h = height(v);
	emissiveColor = mix(purple, blue, h);
    return emissiveColor * (lightingColor / 2);
}

void main()
{
	frag_color = vec4(get_color(vtx_pos.xy),1.f);
}