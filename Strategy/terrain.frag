#version 150
const vec3 light = vec3(1.0, -1.0, 0.0);

uniform sampler2D grass;

in vec3 height;
in vec2 uv;

void main(void)
{
	vec4 col = mix(vec4(0.3, 0.74, 0.2, 1.0), vec4(0.42, 0.26, 0.12, 0.0), height.z);
	gl_FragColor = vec4(height.x, pow(height.y * 0.1, 10.0), 0.0f, 0.0f) + col;
}