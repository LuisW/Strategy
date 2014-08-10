const vec3 light = vec3(0.70711, -0.70711, 0.0);

uniform sampler2D grass;
uniform sampler2D rock;
uniform float texelSize;
uniform sampler2D hmap;
uniform vec3 camPos;

const float mapScale = 819.3;
const float Yscale = 1250.0;

in vec3 pos3D;

vec3 calcNormal(vec2 pos, float delta)
{
	float top =  texture2D(hmap, (pos / mapScale + vec2(0,delta))).x * Yscale;
	float bottom =  texture2D(hmap, (pos / mapScale - vec2(0,delta))).x * Yscale;
	float right =  texture2D(hmap, (pos / mapScale + vec2(delta,0))).x * Yscale;
	float left =  texture2D(hmap, (pos / mapScale - vec2(delta,0))).x * Yscale;

	vec3 normal = vec3(left-right, mapScale*delta*2.0, bottom-top);
	return normalize(normal);
}

void main(void)
{
	vec3 normal = calcNormal(pos3D.xz, texelSize);

	vec4 col = mix(texture2D(rock, pos3D.xz),texture2D(grass, pos3D.xz),  abs(0.7 * normal.y));
	float specular = pow(saturate(dot(normal, normalize(normalize(camPos - pos3D) - light))), 100.0);
	float diffuse = max(dot(-light, normal),0.0);

	gl_FragColor = col * (diffuse + 0.3 + 0.1 * specular);
}

//vec4(0.3, 0.74, 0.2, 1.0)
//vec4(0.42, 0.26, 0.12, 0.0)