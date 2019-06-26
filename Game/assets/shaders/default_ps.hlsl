struct Input
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightDir : NORMAL1;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 main(Input input) : SV_TARGET
{
    float diff = max(dot(input.lightDir, input.normal), 0.0f);

    float4 textureColor = shaderTexture.Sample(SampleType, input.texcoord);

    float4 _color = float4(0.5, 0.5, 0.5, 1);

    //_color = float4(textureColor.rgb*textureColor.a,1);

    //if (textureColor.a == 0)
    //    _color = (_color + float4(input.color, 1.0f))*diff;
    //else
    //    _color = (_color)*diff;

    return float4(input.color,1.0f)*diff;
}