#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texMap;
uniform sampler2D NormalMap;
uniform sampler2D DepthMap;
uniform sampler2D LightMap;
uniform sampler2D SpecMap;

void main()
{
	vec4 Diffuse= texture(texMap, TexCoords.xy);

	if(Diffuse.w < 0.05)
        discard;

    vec4 Light = texture(LightMap, TexCoords);
    vec4 Spec = texture(SpecMap, TexCoords);

 	Light *= 0.6;
 	Spec *= 0.4;
    vec2 texelSize = 1.0 / textureSize(LightMap, 0);

	// vec2 shift = texelSize * vec2(0.0, 1.0);
    // Light += 0.1 * (texture(LightMap,TexCoords + shift));
    // Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));
    // shift =texelSize * vec2(0.0,-1.0);
    // Light += 0.1 * (texture(LightMap,TexCoords + shift));
    // Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));
    // shift =texelSize * vec2(1.0, 0.0);
    // Light += 0.1 * (texture(LightMap,TexCoords + shift));
    // Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));
    // shift = texelSize * vec2(-1.0, 0.0);
    // Light += 0.1 * (texture(LightMap,TexCoords + shift));
    // Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));

	texelSize = 0.7 / textureSize(NormalMap, 0);

	Diffuse.w = 1.0;
	vec4 texColor = Diffuse*vec4(Light.xyz,1.0)+ vec4(Spec.xyz,0.0);

	

	//Light = texture(LightMap, TexCoords);
    //Spec = texture(SpecMap, TexCoords);

	vec3 vt = (texture(NormalMap, TexCoords.xy + vec2(-1, -1)* texelSize).xyz
					-texture(NormalMap, TexCoords.xy + vec2(1, 1)* texelSize ).xyz
					);
	vec3 vt2 = (texture(NormalMap, TexCoords.xy + vec2(-1, 1)* texelSize).xyz
					-texture(NormalMap, TexCoords.xy + vec2(1, -1)* texelSize ).xyz
					);
	vt = 0.5 *(vt + vt2);

	float v_n =dot(vt,vt2);


	float d_depth = abs(
						texture(DepthMap, TexCoords.xy + vec2(-1, -1) * texelSize).x -
						texture(DepthMap, TexCoords.xy + vec2(1, 1) * texelSize).x
					);
	d_depth += abs(
						texture(DepthMap, TexCoords.xy + vec2(-1, 1) * texelSize).x -
						texture(DepthMap, TexCoords.xy + vec2(1, -1) * texelSize).x
					);


	v_n /=v_n + 0.05;

	d_depth /= d_depth +0.01;

	float blur = (1.0-v_n )*(1.0 - d_depth);
	blur = clamp(blur,0.0,1.0);
	//blur =smoothstep(0.0,0.2, blur* blur);
	if(blur< 0.5)
	{
	//blur =smoothstep(0.0,0.2, blur* blur);
		vec2 shift = texelSize * vec2(0.0, 1.0);
		Light += 0.1 * (texture(LightMap,TexCoords + shift));
		Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));
		shift =texelSize * vec2(0.0,-1.0);
		Light += 0.1 * (texture(LightMap,TexCoords + shift));
		Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));
		shift =texelSize * vec2(1.0, 0.0);
		Light += 0.1 * (texture(LightMap,TexCoords + shift));
		Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));
		shift = texelSize * vec2(-1.0, 0.0);
		Light += 0.1 * (texture(LightMap,TexCoords + shift));
		Spec  += 0.15 * (texture(SpecMap,TexCoords + shift));

		vec4 texColor = Diffuse*vec4(Light.xyz,1.0)+ vec4(Spec.xyz,0.0);
		blur =smoothstep(0.0,0.2, blur* blur);

		texColor *= (1.0 - blur);

	}
	
	FragColor = vec4(texColor.xyz, 1.0);

}
