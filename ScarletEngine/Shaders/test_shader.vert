#version 410 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vUV;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 vp;

void main()
{
	FragPos = vec3(model * vec4(vPos.x, vPos.y, vPos.z, 1.0));
	Normal = mat3(transpose(inverse(model))) * vNorm;
	
	gl_Position = vp * vec4(FragPos, 1.0);
}
