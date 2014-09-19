uniform vec3 Color;

varying vec3 normal;

const vec3 light = vec3(0.70711, -0.70711, 0.0);
const vec3 light2 = vec3(0.0f, 0.0f, 1.0f);

void main(void)
{
	gl_FragColor = vec4(Color, 1.0f) * (max(dot(-light, normalize(normal)), 0.0) + 0.3);
}