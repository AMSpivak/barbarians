#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D DiffuseMap;
uniform sampler2D NormalMap;
uniform sampler2D PositionMap;

//uniform mat4 lightSpaceMatrix;

uniform vec4 LightLocation;
uniform vec3 LightColor;

void main()
{
    float rad = LightLocation.w;
	vec4 texColor = texture(DiffuseMap, TexCoords);
    if(texColor.a < 0.1)
        discard;
    vec3 LightVec = LightLocation.xyz - texture(PositionMap, TexCoords).xyz;
    vec3 LightDir = normalize(LightVec);
	vec3 texNormal= texture(NormalMap, TexCoords).xyz;
    float norm_l = max(dot(texNormal,LightDir),0);
    //norm_l = smoothstep(0.45,0.55,norm_l);
    //norm_l *=  max(LightLocation.w - length(LightVec),0);//(smoothstep(LightLocation.w*0.3,LightLocation.w,length(LightDir)));
    norm_l *=  1.0 - smoothstep(rad*0.3,rad,length(LightVec));
    //norm_l = smoothstep(0.45,0.55,norm_l);
    float res = norm_l;

    //float res    = smoothstep(0.45,0.55,norm_l);
    //FragColor =vec4( texColor.xyz,1.0);
    FragColor =vec4(0.6 *  (res) * LightColor * texColor.xyz,1.0);//texColor;// LightDir.y*(0.3 +0.7*(shadow_res) *norm_l) * texColor;
}
