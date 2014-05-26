#version 140

in vec3 in_Position;
in uvec4 in_InstData;

const mat2 rotMats[4] = mat2[4](mat2(1.0), mat2(0,1,-1,0), mat2(-1.0f), mat2(0,-1,1,0));

const uint scaleMask = 255u;

uniform mat4 WVP;
uniform sampler2D hmap;
uniform float gridSize;
uniform float cellLen;
uniform vec2 camPos;

out vec3 height;
out vec2 uv;

void main(void)
{
	vec3 offset = vec3(in_InstData.x, 0.0, in_InstData.y) * gridSize;

	uint scale = (1u << (in_InstData.z & scaleMask));
	uint rot = in_InstData.z >> 8u;

	vec3 pos = in_Position;
	pos.xz = rotMats[rot] * (pos.xz - gridSize * 0.5) + gridSize * 0.5;
	pos = pos * scale + offset;
	pos.y  = height.z = texture2D(hmap, (pos.xz) / (40000.0)).x;
	pos.y = pow(pos.y, 3);
	pos.y *= 2000;

	if(gl_VertexID % 33 == 0 || gl_VertexID < 33)
		height.x = 1.0;
	else
		height.x = 0.0;

	height.y = 1.0 / distance(pos.xz, camPos);

	uv = pos.xz;
	gl_Position = WVP * vec4(pos, 1.0);
}