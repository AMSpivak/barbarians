#version 330 core

#define M_PI 3.1415926535897932384626433832795

//out vec4 FragColor;
in vec2 TexCoords;

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormal;


uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D PositionMap;
uniform sampler2DShadow shadowMap;
//uniform sampler2D shadowMap;

uniform mat4 lightSpaceMatrix;

uniform vec3 LightDir;
uniform vec3 LightColor;
uniform vec3 viewPos;

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
    float res = texture(shadowMap,vec3(projCoords.xy,currentDepth));
//    float res = texture(shadowMap,vec3(projCoords.xy,currentDepth - bias));
    //float depth = texture(shadowMap,projCoords.xy).r;
    //float res = smoothstep(-0.1,0.1,depth - currentDepth);

  //  res = smoothstep(0.0,1.0, (texture(shadowMap,projCoords.xy) - currentDepth + bias));
    res *=0.4;
    res += 0.15*(texture(shadowMap,vec3(projCoords.xy + shift,currentDepth)));
    shift =texelSize*vec2(0.0,-1.0);
    res += 0.15*(texture(shadowMap,vec3(projCoords.xy + shift,currentDepth)));
    shift =texelSize*vec2(1.0, 0.0);
    res += 0.15*(texture(shadowMap,vec3(projCoords.xy + shift,currentDepth)));
    shift = texelSize*vec2(-1.0, 0.0);
    res += 0.15*(texture(shadowMap,vec3(projCoords.xy + shift,currentDepth)));

    //res*=0.2;/**/
    return res;
}

float GGX_PartialGeometry(float cosThetaN, float alpha) {
    float cosTheta_sqr = clamp(cosThetaN*cosThetaN, 0.0, 1.0);
    float tan2 = ( 1 - cosTheta_sqr ) / cosTheta_sqr;
    float GP = 2 / ( 1 + sqrt( 1 + alpha * alpha * tan2 ) );
    return GP;
}

float GGX_Distribution(float cosThetaNH, float alpha) {
    float alpha2 = alpha * alpha;
    float NH_sqr = clamp(cosThetaNH*cosThetaNH, 0.0, 1.0);
    float den = NH_sqr * alpha2 + (1.0 - NH_sqr);
    return alpha2 / ( M_PI * den * den );
}

void main()
{
	vec4 texColor = texture(DiffuseMap, TexCoords);
    if(texColor.a < 0.05)
        discard;
    vec4 normal_map = texture(NormalMap, TexCoords);
	vec3 texNormal= normal_map.xyz;
    //texNormal = normalize(texNormal * 2.0 - 1.0); 
	vec3 FragPos= texture(PositionMap, TexCoords).xyz;
	float norm_l = max(dot(texNormal,LightDir),0);
	//vec3 reflectDir= reflect(-LightDir, texNormal).xyz;
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 halfwayDir = normalize(LightDir + viewDir); 
    float dotHV = max(dot(viewDir, halfwayDir), 0.0);
    float dotNH = max(dot(texNormal, halfwayDir), 0.0);
    float dotNV = max(dot(texNormal, viewDir), 0.0);

    float f0 = mix(0.004,0.7,texColor.a);//texColor.a;
    float shlick =f0 + (1.0-f0)*pow((1.0 -norm_l),5);
    float roug_sqr = (normal_map.w)*(normal_map.w);
    float G = GGX_PartialGeometry(dotNV, roug_sqr) * GGX_PartialGeometry(norm_l, roug_sqr);
    float D = GGX_Distribution(dotNH, roug_sqr);    

    float spec = G*shlick*D*0.25/(max(0.0, dotNV)+0.001);//*dotNV);
    spec =max(0.0, spec);


	float shadow_res =(ShadowCalculation(vec4(FragPos.xyz,1.0),texNormal));
    float shadow_norm =1.0 -  smoothstep(0.6,1.0,1.0-norm_l);//norm_l;//smoothstep(shadow_res,1.0,norm_l);
    shadow_res = 1.0 - (1.0 - shadow_res) * norm_l;
    float diffuse = clamp(1.0 - shlick, 0.0, 1.0);
    float res = shadow_res *(diffuse*norm_l/M_PI);

    //gAlbedoSpec =vec4(vec3(1.0,1.0,1.0),1.0);
    //gNormal =vec4(0.0 * vec3(1.0,1.0,1.0),1.0); 
    gAlbedoSpec =vec4(((res) )* LightColor * vec3(1.0,1.0,1.0),1.0);
    gNormal =vec4(((shadow_res*spec) )* LightColor * vec3(1.0,1.0,1.0),1.0);
}
