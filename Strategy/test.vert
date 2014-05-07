in vec3 in_Position;
in vec3 in_Normal;

uniform mat4 WVP;

varying vec3 normal;

void main(void)
{
	normal = in_Normal;
	gl_Position = WVP * vec4(in_Position, 1.0); 
}