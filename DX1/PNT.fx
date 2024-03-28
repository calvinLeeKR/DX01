//--------------------------------------------------------------------------------------
// File: Tutorial10.fx
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License (MIT).
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

cbuffer cbNeverChanges : register( b0 )
{
    float3 LightDir       : packoffset(c0);
	float  Ambient		   : packoffset( c0.w );
    float3 EyePosition     : packoffset(c1);
};

cbuffer cbChangesEveryFrame : register( b1 )
{
    matrix WorldViewProj;
    matrix World;
    float3x3 WorldInverseTranspose;
};

struct VS_INPUT
{
    float3 Pos          : POSITION;         //position
    float3 Norm         : NORMAL;           //normal
    float2 Tex          : TEXCOORD0;        //texture coordinate
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Diffuse  : COLOR0;
    float3 Specular : COLOR1;
    float2 Tex : TEXCOORD1;
};

struct ColorPair
{
    float3 Diffuse;
    float3 Specular;
};

ColorPair ComputeLights(float3 V, float3 N)
{
    float3 L = normalize(LightDir);
    float3 H = normalize(V + L);;

    float NdotL = saturate(dot(N, L));
    float NdotH = saturate(dot(N, H));

    float SpecPower = pow(max(NdotH, 0), 256);

    ColorPair result;

    result.Diffuse  = NdotL + Ambient;
    result.Specular = SpecPower;

    return result;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    float4 pos_ws = mul( float4(input.Pos,1), World );
    float3 N = normalize(mul(input.Norm, (float3x3)World));
    float3 V = normalize(EyePosition - pos_ws.xyz);

    ColorPair result = ComputeLights(V, N);
    
    output.Pos = mul(float4(input.Pos, 1), WorldViewProj);;
    output.Specular = result.Specular;
    output.Diffuse = float4(result.Diffuse,1);
    output.Tex = input.Tex;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    //calculate lighting assuming light color is <1,1,1,1>
    float4 outputColor = txDiffuse.Sample( samLinear, input.Tex ) * input.Diffuse;
    outputColor.rgb += input.Specular;
    outputColor.a = 1;
    return outputColor;
}
