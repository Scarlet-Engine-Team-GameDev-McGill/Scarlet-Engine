#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

vec3 ObjectColor = vec3(1.0, 0.12, 0.0);

void main()
{
    outColor = vec4(ObjectColor, 1.0);
}