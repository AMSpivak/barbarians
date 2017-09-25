#version 330 core
in vec3 ourColor;
in vec3 ourColorView;
in vec3 v_Position;
in vec2 TexCoord;

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec3 gPosition;
uniform sampler2D ourTexture;

void main()
{

    gPosition = v_Position;
	vec4 texColor = texture(ourTexture, TexCoord);
    if(texColor.a < 0.1)
        discard;
    gNormal = vec4(ourColor.xyz, 1.0);
	//gNormal = vec4(ourColor.xyz, 1.0);
	gAlbedoSpec = texColor;
}