float2 src = (0,0);

struct PS_INPUT
{
	vector UV : TEXCOORD0;
	vector Color : COLOR0;
};
sampler2D s_2D;

float4 Render( PS_INPUT input ) : COLOR0
{
	float2 r = input.UV - src;
	r = (1,1);
	float d = input.UV.x + input.UV.y;
	r = r / length(r);
	float2 uv = input.UV + (sin(input.UV.y * 50), sin(input.UV.x * 50)) / 100;
	return (tex2D(s_2D, uv)) * input.Color.a;
}
