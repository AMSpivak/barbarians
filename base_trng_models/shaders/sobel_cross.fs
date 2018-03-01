#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texMap;
uniform sampler2D NormalMap;
uniform sampler2D DepthMap;
uniform sampler2D LightMap;

void main()
{
	vec4 Diffuse= texture(texMap, TexCoords.xy);

	if(Diffuse.w < 0.05)
        discard;

    vec4 Light = texture(LightMap, TexCoords);


	vec2 texelSize = 0.7 / textureSize(NormalMap, 0);

	vec3 vt = (texture(NormalMap, TexCoords.xy + vec2(-1, -1)* texelSize).xyz
					-texture(NormalMap, TexCoords.xy + vec2(1, 1)* texelSize ).xyz
					);
	vec3 vt2 = (texture(NormalMap, TexCoords.xy + vec2(-1, 1)* texelSize).xyz
					-texture(NormalMap, TexCoords.xy + vec2(1, -1)* texelSize ).xyz
					);
	vt = 0.5 *(vt + vt2);

	float v_n =dot(vt,vt2);
					;
	//v_n = max(v_n,0.0);
			//vx += 0.5 *  texture(NormalMap, TexCoords.xy + vec2(x, y) * texelSize).xyz;
			//vy += 0.5 *  texture(NormalMap, TexCoords.xy + vec2(x, y) * texelSize).xyz;

	float d_depth = abs(
						texture(DepthMap, TexCoords.xy + vec2(-1, -1) * texelSize).x -
						texture(DepthMap, TexCoords.xy + vec2(1, 1) * texelSize).x
					);
	d_depth += abs(
						texture(DepthMap, TexCoords.xy + vec2(-1, 1) * texelSize).x -
						texture(DepthMap, TexCoords.xy + vec2(1, -1) * texelSize).x
					);


	v_n /=v_n + 0.3;

	d_depth /= d_depth +0.01;
	//v = 0.25 *v;
	//v = pow(v,4);
	//FragColor  =/* (1.0-v_n )*(1.0 - d_depth)**/texture(texMap, TexCoords);
	FragColor  = (1.0-v_n )*(1.0 - d_depth)*texture(texMap, TexCoords)*vec4(Light.xyz,1.0);
	//vec4(v,v,v, 1.0);
	Diffuse.w = 1.0;
	vec4 texColor = Diffuse*vec4(Light.xyz,1.0);
    FragColor = vec4(texColor.xyz, 1.0);
}
