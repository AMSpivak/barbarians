#version 330 core
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D PositionMap;


void main()
{

	float v  = texture(PositionMap, TexCoords).w;
    FragColor =vec4(v,v,v,1.0);//texColor;// LightDir.y*(0.3 +0.7*(shadow_res) *norm_l) * texColor;
}
