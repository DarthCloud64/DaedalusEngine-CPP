struct PSInput
{
	float4 Position : SV_POSITION;
};

float4 main(PSInput pin) : SV_TARGET
{
	return float4(1.0, 0.5, 0.2, 1.0);
}
