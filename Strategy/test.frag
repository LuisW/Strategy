varying vec3 normal;

const vec3 light = vec3(1.0, -1.0, 0.0);

void main(void)
{
	gl_FragColor = vec4(1.0) * dot(normalize(-light), normalize(normal));
}