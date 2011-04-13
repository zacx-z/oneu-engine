float lPos;
sampler2D s_2D;
float4 Render( float2 UV : TEXCOORD0 ) : COLOR0
{
	float h = 0.0f;
	if( sin( UV.y * 500 ) > 0.5f ) h = 0.2f;
	if( UV.y > lPos - 0.2f && UV.y < lPos )
		h += ( UV.y + 0.2f - lPos ) * 5;
	if( UV.y > lPos - 0.01f && UV.y < lPos )
		h += ( UV.y + 0.05f - lPos ) * 100;
	return tex2D( s_2D, UV ) + float4( h * 2 / 3, h * 2 / 3, h, 0.0f );
}