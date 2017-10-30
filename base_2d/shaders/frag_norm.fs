#version 330 core
in vec3 ourColor;
in vec3 ourColorView;
in vec3 v_Position;
in vec2 TexCoord;

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gPosition;
uniform sampler2D ourTexture;
uniform sampler2D UtilityTexture;
void main()
{

	vec4 texColor = texture(ourTexture, TexCoord);
    if(texColor.a < 0.1)
        discard;
    vec4 pos = vec4(v_Position,texture(UtilityTexture, TexCoord).x);
    gPosition = pos;
    gNormal = vec4(ourColor.xyz, 1.0);
	//gNormal = vec4(ourColor.xyz, 1.0);
	gAlbedoSpec = texColor;
}