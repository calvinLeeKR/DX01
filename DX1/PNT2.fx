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
    float3 LightDir       : packoffset( c0 );
	float  Ambient		   : packoffset( c0.w );    
};

cbuffer cbChangesEveryFrame : register( b1 )
{
    matrix WorldViewProj;
    matrix World;
};

struct VS_INPUT
{
    float3 Pos          : POSITION;         //position
    float3 Norm         : NORMAL;           //normal
    float2 Tex          : TEXCOORD0;        //texture coordinate
};

struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
	float3 Norm	    : NORMAL;
    float2 Tex      : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    output.Pos = mul( float4(input.Pos,1), WorldViewProj );
    output.Norm = normalize( mul( input.Norm, (float3x3)World ) );
    output.Tex = input.Tex;
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 TexColor = txDiffuse.Sample( samLinear, input.Tex );
    float Lighting = saturate( dot( LightDir, input.Norm ) );
    Lighting = max( Lighting, Ambient );
    return TexColor * Lighting;
}
