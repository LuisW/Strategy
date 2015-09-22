#version 150

#define PiOver2 1.57079632679
#define TwoPiOver256 0.0245436926
#define hmapres 4097u
#define nFreq 12u

in vec2 Coord;
out float out_Height;

uint hash(uint seed)	//http://www.reedbeta.com/blog/2013/01/12/quick-and-easy-gpu-random-numbers-in-d3d11/
{
    seed = (seed ^ 61u) ^ (seed >> 16u);
    seed *= 9u;
    seed = seed ^ (seed >> 4u);
    seed *= 668265261u;
    seed = seed ^ (seed >> 15u);
    return seed % 256u;
}

vec2 getGradient(uint x, uint y, uint freq, uint seed)
{
	float phi = float(hash(nFreq * hmapres * x + nFreq * y + freq + seed));
	phi = TwoPiOver256 * phi;
	return vec2(cos(phi), sin(phi));
}

float fade(float t)
{
	 return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float fade_dt(float t)
{
	 return t * t * (t * (t * 30.0 - 60.0) + 30.0);
}

float tfade_dt(float t)
{
	 return t * t * t * (t * (t * 36.0 - 75.0) + 40.0);
}

float perlin(vec2 pos, uint freq, uint seed, out vec2 grad)
{
	uvec2 cell = uvec2(pos);

	vec2 g00 = getGradient(cell.x     , cell.y     , freq, seed);
	vec2 g10 = getGradient(cell.x + 1u, cell.y     , freq, seed);
	vec2 g01 = getGradient(cell.x     , cell.y + 1u, freq, seed);
	vec2 g11 = getGradient(cell.x + 1u, cell.y + 1u, freq, seed);

	vec2 fr = fract(pos);

	vec2 d00 = fr;
	vec2 d01 = vec2(fr.x, fr.y - 1.0);
	vec2 d10 = vec2(fr.x - 1.0, fr.y);
	vec2 d11 = fr - 1.0;

	float p00 = dot(g00, d00);
	float p01 = dot(g01, d01);
	float p10 = dot(g10, d10);
	float p11 = dot(g11, d11);

	vec2 ease = vec2(fade(fr.x), fade(fr.y));
	vec2 ease_dt = vec2(fade_dt(fr.x), fade_dt(fr.y));
	vec2 tease_dt = vec2(tfade_dt(fr.x), tfade_dt(fr.y));

	float c0 = p11 - p01 - p10 + p00;
	float c1 = p10 - p00;
	float c2 = p01 - p00;
	float c3 = p00;

	grad.x =	tease_dt.x * (ease.y * (g00.x + g11.x - g10.x - g01.x) + (g10.x - g00.x)) + 
				ease_dt.x * ( fr.y * (ease.y * (g00.y + g11.y - g10.y - g01.y) + g10.y - g00.y) + ease.y * (-g11.x - g11.y + g10.x + g01.y) - g10.x) +
				ease.y * (g01.x - g00.x) + g00.x;

	grad.y =	tease_dt.y * (ease.x * (g00.y + g11.y - g10.y - g01.y) + (g01.y - g00.y)) + 
				ease_dt.y * ( fr.x * (ease.x * (g00.x + g11.x - g10.x - g01.x) + g01.x - g00.x) + ease.x * (-g11.x - g11.y + g10.x + g01.y) - g01.y) +
				ease.x * (g10.y - g00.y) + g00.y;

	return c3 + c2 * ease.y + c1 * ease.x + c0 * ease.x * ease.y;
}

float FBMNoise(vec2 pos, uint freqCnt, float persistence)
{
	float h = 0.0;
	float amp = 1.0;
	float refAmp = 1.0;
	float norm = 0.0;
	float sz = 1.0 / float(1u<<freqCnt);

	vec2 grad = vec2(0.0);
	vec2 tgrad;

	for(uint n = freqCnt; n >= freqCnt - 4u; n = n-1u)
	{
		h += refAmp * perlin(pos * sz, n, 0u, tgrad);
		grad += tgrad * sz * amp * 200.0;
		norm += refAmp;
		refAmp *= persistence;
		sz *= 2.0;
	}

	amp = min(length(grad), 1.0);
	amp = pow(amp, 2.0);

	for(uint n = freqCnt - 5u; n >= freqCnt - 12u; n = n-1u)
	{
		h += refAmp * amp * perlin(pos * sz, n, 0u, tgrad);
		norm += refAmp;
		refAmp *= persistence;
		sz *= 2.0;
	}

	return 0.5 * (h / norm + 1.0);
}

void main()
{
	vec2 grad;
	float persistence = perlin(Coord, 10u, 0u, grad);
	out_Height = FBMNoise(Coord, nFreq, 0.55);
}