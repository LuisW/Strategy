#version 150

uniform vec3 cspec;
uniform vec3 cdiff;
uniform vec3 camb;
uniform float roughness;
uniform sampler2D norMap;

in vec2 UV;
in vec3 toLight;
in vec3 toCam;
in mat3 TBN;

const vec3 clight = vec3(5.0, 5.0, 5.0);
const float lightRadius = 10.0;

out vec4 outColor;

const float overPi = 0.31830988618;

void main(void)
{
	float sq_dist = dot(toLight, toLight);

	float falloff = max(1 - (sq_dist * sq_dist) / pow(lightRadius, 4.0), 0.0) / (sq_dist + 1.0);

	vec3 N = normalize(TBN * texture2D(norMap, UV).xyz);
	vec3 V = normalize(toCam);
	vec3 L = normalize(toLight);
	vec3 H = normalize(L+V);

	float NH = dot(N, H);
	float LH = dot(L, H);
	float NV = dot(N, V);
	float NL = max(dot(N, L), 0.0);

	float sq_r = roughness * roughness;
	float f1 = NH * NH * (sq_r - 1.0) + 1;
	float NDF = sq_r / (f1 * f1);

	vec3 schlick = cspec + (1.0 - cspec) * pow(1.0 - LH, 5.0);
	vec3 diff_schlick = cspec + (1.0 - cspec) * pow(1.0 - NL, 5.0);

	sq_r = (roughness+1.0)/2.0;
	sq_r *= sq_r;

	float G_V = NV + sqrt( (NV - NV * sq_r) * NV + sq_r );
	float G_L = NL + sqrt( (NL - NL * sq_r) * NL + sq_r );
	float smithGGX = 1.0/( G_V * G_L );

	vec3 spec = schlick * NDF * smithGGX;
	vec3 diff = cdiff * (1.0-diff_schlick);

	vec3 light = clight * falloff;

	outColor = vec4(camb * cdiff + (spec + diff) * light * NL, 1.0);
	outColor = texture2D(norMap, 5.0*UV);
}