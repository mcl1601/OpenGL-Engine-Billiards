#version 430

in vec3 texDirection;
uniform samplerCube cubemap;

void main()
{
	vec4 color = texture(cubemap, texDirection);
	gl_FragColor = color;
}