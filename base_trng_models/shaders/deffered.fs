#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D PositionMap;
uniform sampler2DShadow shadowMap;
//uniform sampler2D shadowMap;

uniform mat4 lightSpaceMatrix;

uniform vec3 LightDir;
uniform vec3 LightColor;

float ShadowCalculation(vec4 PosLight, vec3 tNormal)
{
	vec4 fragPosLightSpace = lightSpaceMatrix * PosLight;


	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	float bias = max(0.01 * smoothstep(0.65,0.85,(1.0 - max(dot(tNormal,LightDir),0.0))), 0.005);

    vec2 shift = texelSize*vec2(0.0,1.0);
    //float res = (textureProj(shadowMap,vec4(projCoords,1.0)));
    float res = texture(shadowMap,vec3(projCoords.xy,currentDepth - bias));
    //float depth = texture(shadowMap,projCoords.xy).r;
    //float res = smoothstep(-0.1,0.1,depth - currentDepth);

    //res = smoothstep(0.0,1.0, (texture(shadowMap,projCoords.xy) - currentDepth + bias));

    res += (texture(shadowMap,vec3(projCoords.xy + shift,currentDepth - bias)));
    shift =texelSize*vec2(0.0,-1.0);
    res += (texture(shadowMap,vec3(projCoords.xy + shift,currentDepth - bias)));
    shift = texelSize*vec2(1.0, 0.0);
    res += (texture(shadowMap,vec3(projCoords.xy + shift,currentDepth - bias)));
    shift = texelSize*vec2(-1.0, 0.0);
    res += (texture(shadowMap,vec3(projCoords.xy + shift,currentDepth - bias)));

    res*=0.2;/**/
    return res;
}

void main()
{
	vec4 texColor = texture(DiffuseMap, TexCoords);
    if(texColor.a < 0.1)
        discard;
	vec3 texNormal= texture(NormalMap, TexCoords).xyz;
	float norm_l = max(dot(texNormal,LightDir),0);
    //float norm_l = smoothstep(0.45,0.55,dot(texNormal,LightDir));

    //float norm_l = smoothstep(0.0,1.0,dot(texNormal,LightDir));
    //float norm_l = max(dot(texNormal,LightDir),0.0);
	float shadow_res =(ShadowCalculation(vec4(texture(PositionMap, TexCoords).xyz,1.0),texNormal));
    shadow_res = smoothstep(0.0,0.99, shadow_res);
    //float res = min((shadow_res), norm_l);// * norm_l);//min(shadow_res,norm_l);
    float res = shadow_res* norm_l;// * norm_l);//min(shadow_res,norm_l);
    res    = smoothstep(0.45,0.55,res);
    FragColor =vec4(((res) )* LightColor * vec3(1.0,1.0,1.0),1.0);//texColor;// LightDir.y*(0.3 +0.7*(shadow_res) *norm_l) * texColor;

    //FragColor =vec4(0.4 +0.6 *  (res) * LightColor,1.0);//texColor;// LightDir.y*(0.3 +0.7*(shadow_res) *norm_l) * texColor;
    //FragColor =  vec4((0.3 +0.7 *  (norm_l)) * texColor.xyz,1.0);//texColor;// LightDir.y*(0.3 +0.7*(shadow_res) *norm_l) * texColor;
}
