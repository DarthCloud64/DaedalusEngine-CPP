struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
};


float4 main(PSInput pin)
{
	return pin.Color;
}
