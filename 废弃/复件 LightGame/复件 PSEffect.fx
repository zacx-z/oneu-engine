Texture tex;

float4 color = { 0.0f, 0.0f, 1.0f, 1.0f };
sampler2D s_2D;

struct PS_INPUT
{
	vector UV : TEXCOORD0;
	vector Color : COLOR0;
};
static const int g_cKernelSize = 6;
float2 PixelKernel[g_cKernelSize] =
{
    { -1, 0 },
    { -2, 0 },
    { -3, 0 },
    { -4, 0 },
    { -5, 0 },
    { -6, 0 }
};


float4 Render( PS_INPUT input ) : COLOR0
{
	float2 tUV = float2( input.UV.x - 0.01f, input.UV.y - 0.01f );
	float4 curColor = tex2D( s_2D, input.UV );
	float4 del = ( curColor - tex2D( s_2D, tUV ) );
	float h = del.r * 0.3 + del.g * 0.59 + del.b * 0.11;
	return ((float4( h,h,h,0.0f ) + float4( 0.5,0.5,0.5, curColor.a)) + (input.Color.rgb, 0)) * input.Color.a;
}
float MasaikeSize = 50.0f;
float4 RenderMasaike( PS_INPUT input ) : COLOR0
{
	float2 UV = int2( input.UV.x * MasaikeSize, input.UV.y * MasaikeSize ) / MasaikeSize;
	return tex2D( s_2D, UV );
}
