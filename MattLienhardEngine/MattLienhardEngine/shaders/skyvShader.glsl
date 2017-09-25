#version 430

layout (location = 0) in vec3 modelLoc;
layout (location = 1) uniform mat4 cameraMat;
out vec3 texDirection;

void main()
{
	gl_Position = vec4(modelLoc * 20f, 1);
	texDirection = modelLoc;
}