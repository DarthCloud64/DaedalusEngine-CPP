struct VSInput
{
	float3 Position : ATTRIB0;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
};

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = float4(vin.Position, 1.0);

	return vout;
}