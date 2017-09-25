#version 430

layout (location = 0) in vec3 modelLoc;
layout (location = 1) uniform mat4 cameraMat;
layout (location = 7) in vec3 vertNorm;
layout (location = 8) in vec2 modelUV;
layout (location = 15) uniform mat4 modelWorldMat;
out vec2 fragUV;
out vec3 fragLoc;
out vec3 fragNorm;
void main()
{
	gl_Position = cameraMat * modelWorldMat * vec4(modelLoc, 1);
	fragUV = modelUV;
	fragLoc = modelLoc;
	fragNorm =  vec3(transpose(inverse(modelWorldMat)) * vec4(vertNorm, 0));
}