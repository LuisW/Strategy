const vec3 light = vec3(0.70711, -0.70711, 0.0);

uniform sampler2D grass;
uniform sampler2D rock;
uniform float texelSize;
uniform float cellLen;
uniform sampler2D hmapNor;
uniform vec3 camPos;

const float Yscale = 200.0;

in vec3 pos3D;
in vec2 samplePos;
in float cellScale;

vec3 calcNormal(vec2 pos, float delta, float scale)
{
	float top =  texture2D(hmapNor, (pos + vec2(0,delta))).x * Yscale;
	float bottom =  texture2D(hmapNor, (pos - vec2(0,delta))).x * Yscale;
	float right =  texture2D(hmapNor, (pos + vec2(delta,0))).x * Yscale;
	float left =  texture2D(hmapNor, (pos - vec2(delta,0))).x * Yscale;

	vec3 normal = vec3(left-right, scale*cellLen*2.0, bottom-top);
	return normalize(normal);
}

void main(void)
{
	vec3 normal = calcNormal(samplePos, texelSize, cellScale);

	vec4 col = mix(0.5 * (texture2D(rock, pos3D.xz) + vec4(75.0 / 255.0, 56.25 / 255.0, 32.0 / 255.0, 1.0)) ,texture2D(grass, pos3D.xz), pow(abs(normal.y), 10.0));
	float specular = pow(clamp(dot(normal, normalize(normalize(camPos - pos3D) - light)), 0.0, 1.0), 10.0);
	float diffuse = max(dot(-light, normal),0.0);

	gl_FragColor = col * (diffuse + 0.3 + 0.1 * specular);
}

//vec4(0.3, 0.74, 0.2, 1.0)
//vec4(0.42, 0.26, 0.12, 0.0)