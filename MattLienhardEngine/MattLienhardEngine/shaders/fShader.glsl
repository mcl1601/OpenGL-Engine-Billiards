#version 430

in vec2 fragUV;

layout (location = 3) uniform float time;

layout (location = 4) uniform vec3 lightLoc;
layout (location = 5) uniform vec3 cameraLoc;

in vec3 fragLoc;
in vec3 fragNorm;

layout (location = 6) uniform vec4 material;

vec3 L = normalize(lightLoc - fragLoc);
vec3 E = normalize(cameraLoc - fragLoc);
vec3 H = normalize(L + E);
vec3 N = normalize(fragNorm);

float diffuse = 0.95f * max(dot(L,N), 0);
float specular = 0.75f * (pow(max(dot(H, N), 0), 16));

float totalLight = 0.5f + diffuse + specular;

uniform sampler2D myTexture;

vec3 lighting(vec2 pos)
{
	vec3 color;
	color.x = totalLight;
	color.y = totalLight;
	color.z = totalLight;
	return color;
}

vec3 rainbow(vec2 pos)
{
	vec3 color;
	float lightness = (pos.y + 0.9) / 1.8;
	if(time < (3.1415926535 / 2))
	{
		color.x = 1 * totalLight;
		color.y = sin(time) * totalLight;
		color.z = 0;
		return color;
	}
	if(time < 3.1415926535)
	{
		color.x = cos(time - (3.1415926535 / 2)) * totalLight;
		color.y = 1 * totalLight;
		color.z = 0;
		return color;
	}
	if(time < 3 * ((3.1415926535 / 2)))
	{
		color.x = 0;
		color.y = 1 * totalLight;
		color.z = sin(time - 3.1415926535) * totalLight;
		return color;
	}
	if(time < (2 * 3.1415926535))
	{
		color.x = 0;
		color.y = cos(time - ((3 * 3.1415926535) / 2)) * totalLight;
		color.z = 1 * totalLight;
		return color;
	}
	if(time < ((2 * 3.1415926535)) + (3.1415926535 / 2))
	{
		color.x = sin(time) * totalLight;
		color.y = 0;
		color.z = 1 * totalLight;
		return color;
	}
	if(time < ((2 * 3.1415926535)) + 3.1415926535 + 1)
	{
		color.x = 1 * totalLight;
		color.y = 0;
		color.z = cos(time - (3.1415926535 / 2)) * totalLight;
		return color;
	}
}

void main()
{
	vec4 color = texture(myTexture, fragUV);
	color.x *= totalLight;
	color.y *= totalLight;
	color.z *= totalLight;
	gl_FragColor = color;
}

