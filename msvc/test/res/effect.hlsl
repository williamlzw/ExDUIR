cbuffer constants : register(b0) {
	float3 color : packoffset(c0);
	float u_time : packoffset(c0.w);
        float2 u_resolution : packoffset(c1);
}

float4 main ( 
	float4 fragCoord  : SV_POSITION,
	float4 sceneSpaceOutput : SCENE_POSITION,
	float4 texelSpaceInput0 : TEXCOORD0
       ) : SV_TARGET {
	   
	float2 uv = fragCoord.xy / u_resolution.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= u_resolution.x/u_resolution.y;
    
    float time = u_time;
   
    float dur = sin(time*0.5)*0.2+1.7;
    float t = fmod(time,dur)/dur;
    float m = sin(time*5.0)*3.0 + sin(time*3.0)*1.0 + sin(time*7.5)*5.0;
    m *= 0.5;
    
    
    float dist = distance(uv, float2(0.0,0.0) );
    float col = sin(6.2831*4.4*t*dist*1.0 - time*30.0)*exp(-m*t)*0.5+0.5;
    col = 1.0/dist * col*2.0;
    
	return float4(col,col,col,1.0);
}