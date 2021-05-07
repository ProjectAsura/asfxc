#include "a.hlsli"

#if 0
#include "a.hlsli"
#endif

VSOutput VSFunc_B(const VSInput input)
{
    VSOutput output = (VSOutput)0;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;
    
    return output;
};

float4 PSFunc_B(VSOutput input) : SV_TARGET0
{
    return textureA.Sample(samplerA, input.texcoord);
};

technique B
{
    pass P0 
    {
         VertexShader = compile vs_6_0 VSFunc_B();
         PixelShader = compile ps_6_0 PSFunc_B();
    }
}
