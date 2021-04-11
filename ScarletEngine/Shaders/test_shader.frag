#version 450 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

vec3 ObjectColor = vec3(0.7, 1.0, 1.0);
vec3 LightColor = vec3(1.0, 1.0, 1.0);
vec3 LightPos = vec3(0.0, 5.0, 2.0);

uniform vec3 CameraPos;

void main()
{
	// ambient
	float AmbientStrength = 0.3;
	vec3 Ambient = AmbientStrength * LightColor;

	// diffuse 
	float DiffuseStrength = 0.5;
	vec3 Norm = normalize(Normal);
	vec3 LightDir = normalize(LightPos - FragPos);
	float Diff = max(dot(Norm, LightDir), 0.0);
	vec3 Diffuse = Diff * DiffuseStrength * LightColor;

	// specular
	float SpecularStrength = 0.5;
	int SpecularIntesity = 32;
	vec3 ViewDir = normalize(CameraPos - FragPos);
	vec3 ReflectDir = reflect(-LightDir, Norm);
	float Spec = pow(max(dot(ViewDir, ReflectDir), 0.0), SpecularIntesity);
	vec3 Specular = Spec * SpecularStrength * LightColor;

	vec3 Result = (Ambient + Diffuse + Specular) * ObjectColor;
	FragColor = vec4(Result, 1.0);
}
