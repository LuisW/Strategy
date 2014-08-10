#version 150

in vec3 in_Position;

out uvec2 Coord;

void main(void)
{
	Coord = uvec2(((in_Position.xy + 1.0) / 2.0) * 2048.0);
	gl_Position = vec4(in_Position, 1.0);
}