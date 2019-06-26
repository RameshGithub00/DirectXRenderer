struct Input
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
    //float3 instancePos : INSTANCEPOS;
    //int instanceId : InstanceID;
};

struct Output
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 lightDir : NORMAL1;
};

cbuffer VS_CONSTANT_BUFFER:register(b0)
{
    float fTime;
};

cbuffer MatrixBuffer:register(b1)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

Output main(Input input)
{
    Output output;
 
    output.pos = input.pos;// +2.0f*float4(input.instancePos, 0);
    output.pos.w = 1.0f;

    output.pos = mul(output.pos, worldMatrix);
    
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);

    output.normal = mul(input.normal, worldMatrix);
    output.normal = normalize(output.normal);

    output.lightDir = normalize(float3(1, -1, 1));

    output.texcoord = float2(-input.pos.x/100,-input.pos.z/100);

    output.color = float3(0.5,0.5,0.5);

    return output;
}