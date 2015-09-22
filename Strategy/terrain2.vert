#version 140

in vec3 in_Position;
in uvec4 in_InstData;
in float in_LoDLerp;

const mat2 rotMats[4] = mat2[4](mat2(1.0), mat2(0,1, -1,0), mat2(-1.0f), mat2(0,-1, 1,0));
const vec2 rotShift[4] = vec2[4](vec2(0.0), vec2(1.0,0.0), vec2(1.0), vec2(0.0, 1.0));
const vec2 neighbours[4] = vec2[4](vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0));

const uint Mask8bit = 255u;
const uint Mask4bit = 15u;
const float Yscale = 200.0;

uniform mat4 WVP;
uniform sampler2D hmap;
uniform float gridSize;
uniform float cellLen;
uniform float texelSize;

out vec3 pos3D;
out vec2 samplePos;
out float cellScale;

void main(void)
{
	vec3 offset = vec3(int(in_InstData.x) - 32768, 0.0, int(in_InstData.y) - 32768) * gridSize;

	uint scale = (1u << (in_InstData.z & Mask8bit));
	uint rot = in_InstData.z >> 8u;

	cellScale = float(scale);

	vec3 pos = in_Position;
	pos.xz = rotMats[rot] * pos.xz + rotShift[rot] * gridSize;
	vec2 uv = vec2(in_InstData.w / 50u, in_InstData.w % 50u); 
	uv = pos.xz / cellLen * min(cellScale, 4.0) + uv * 133.0 + vec2(in_InstData.xy & 3u) * 32.0 + 2.5;
	uv *= texelSize;
	pos = pos * scale + offset;

	pos.y = Yscale * (texture2D(hmap, uv).x);

	//int LoDCase = (gl_VertexID / 33) % 2 + 2 * ((gl_VertexID % 33) % 2);
	//float LowerLoDHeight = Yscale * 0.5 * (texture2D(hmap, (pos.xz + neighbours[LoDCase] * cellLen * scale) / mapScale).x + texture2D(hmap, (pos.xz - neighbours[LoDCase] * cellLen * scale) / mapScale).x);
	//pos.y = mix(pos.y, LowerLoDHeight, in_LoDLerp);
	
	cellScale = max(cellScale * 0.25, 1.0);
	samplePos = uv;
	pos3D = pos;
	gl_Position = WVP * vec4(pos, 1.0);
}