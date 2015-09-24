#version 150

in vec3 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec3 in_Tangent;

uniform mat4 eng_WVP;
uniform mat4 eng_WV;
uniform mat4 eng_V;
uniform mat3 eng_NMat;

out vec2 UV;
out vec3 toLight;
out vec3 toCam;
out mat3 TBN;
out vec3 world;

const vec4 light = vec4(4.0, 0.0, 0.0, 1.0);

void main(void)
{
	UV = in_TexCoord;

	world = vec3(dot(in_Tangent, in_Normal));

	vec3 normal = normalize(eng_NMat * in_Normal);
	vec3 tangent = normalize(eng_NMat * in_Tangent);			//Check you Space Privilege!
	vec3 binormal = normalize(cross(normal, tangent));

	TBN = mat3(tangent, binormal, normal);

	vec3 VPos = (eng_WV * vec4(in_Position, 1.0)).xyz;

	toLight = (eng_V * light).xyz - VPos;
	toCam = -VPos; 

	gl_Position = eng_WVP * vec4(in_Position, 1.0); 
}