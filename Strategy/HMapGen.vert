#version 150

#define hmapres 128u

in vec3 in_Position;
out vec2 Coord;

uniform uvec2 off;

void main(void)
{
	Coord = (in_Position.xy + 1.0) * 2048.0 + in_Position.xy * 0.5 + vec2(off);
	gl_Position = vec4(in_Position, 1.0);
}