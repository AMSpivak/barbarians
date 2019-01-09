#version 330 core
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec4 corrector;



in  vec2 v_Tex;
void main()
{
    float level = 100.0f;
    vec2 v_cl_tex = vec2(clamp(v_Tex.x,0.0,1.0),clamp(v_Tex.y,0.0,1.0));
    FragColor =  corrector*(textureLod(u_Texture, v_cl_tex,level));
    
}