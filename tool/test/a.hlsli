struct VSInput
{
    float3 position : POSITION;
    float2 texcorod : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcorod : TEXCOORD0;
};

Texture2D textureA : register(t0);
SamplerState samplerA : register(s0);

VSOutput VSFunc_A(const VSInput input)
{
    VSOutput output = (VSOutput)0;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;
    
    return output;
};

float4 PSFunc_A(VSOutput input) : SV_TARGET0
{
    return textureA.Sample(samplerA, input.texcoord);
};

technique A
{
    pass P0 
    {
         VertexShader = compile vs_6_0 VSFunc_A();
         PixelShader = compile ps_6_0 PSFunc_A();
    }
}

