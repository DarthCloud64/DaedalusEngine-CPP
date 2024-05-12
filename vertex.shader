struct VSInput
{
	float3 Position : ATTRIB0;
	float4 Color : ATTRIB1;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
};

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = float4(vin.Position, 1.0);
	vout.Color = vin.Color;

	return vout;
}