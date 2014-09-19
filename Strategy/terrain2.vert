#version 140

in vec3 in_Position;
in uvec4 in_InstData;
in float in_LoDLerp;

const mat2 rotMats[4] = mat2[4](mat2(1.0), mat2(0,1, -1,0), mat2(-1.0f), mat2(0,-1, 1,0));
const vec2 neighbours[4] = vec2[4](vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0));

const uint scaleMask = 255u;
const float mapScale = 8192.0;
const float Yscale = 1250.0;

uniform mat4 WVP;
uniform sampler2D hmap;
uniform float gridSize;
uniform float cellLen;

out vec3 pos3D;
out vec3 normal;

void main(void)
{
	vec3 offset = vec3(in_InstData.x, 0.0, in_InstData.y) * gridSize;

	uint scale = (1u << (in_InstData.z & scaleMask));
	uint rot = in_InstData.z >> 8u;

	vec3 pos = in_Position;
	pos.xz = rotMats[rot] * (pos.xz - gridSize * 0.5) + gridSize * 0.5;
	pos = pos * scale + offset;

	//int LoDCase = (gl_VertexID / 33) % 2 + 2 * ((gl_VertexID % 33) % 2);
	//float LowerLoDHeight = Yscale * 0.5 * (texture2D(hmap, (pos.xz + neighbours[LoDCase] * cellLen * scale) / mapScale).x + texture2D(hmap, (pos.xz - neighbours[LoDCase] * cellLen * scale) / mapScale).x);
	//pos.y = mix(pos.y, LowerLoDHeight, in_LoDLerp);

	pos.y = Yscale * (texture2D(hmap, (pos.xz) / mapScale).x);

	pos3D = pos;
	gl_Position = WVP * vec4(pos, 1.0);
}