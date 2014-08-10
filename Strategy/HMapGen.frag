#version 150

in uvec2 Coord;
out vec4 FragColor;

const float OctWeights[9] = float[9](1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);

const int off = 2048;		
					
float noise(uint seed)
{
    seed = (seed ^ 61u) ^ (seed >> 16u);
    seed *= 9u;
    seed = seed ^ (seed >> 4u);
    seed *= 668265261u;
    seed = seed ^ (seed >> 15u);
    return float(seed % 65535u) / 65535.0;
}

void main()
{
	float h = noise(uint(gl_FragCoord.x * 2048.0 + gl_FragCoord.y));
	
	//for(uint n = 2u; n < 4u; n += 2u)
	//{
	//	vec2 p = vec2(gl_FragCoord.xy) / float(1u << n);
	//	h += noise(p) * OctWeights[(n + 1u) / 2u];
	//}

	FragColor = vec4(vec3(h),1.0);
}