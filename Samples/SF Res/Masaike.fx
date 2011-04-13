float HSize = 0.01f;
float VSize = 0.01f;

sampler2D s_2D;

float4 Render( vector UV : TEXCOORD0 ) : COLOR0
{
	float2 tUV;
	tUV.x = (int)( UV.x / VSize ) * VSize;
	tUV.y = (int)( UV.y / HSize ) * HSize;
	return tex2D( s_2D, tUV );
}