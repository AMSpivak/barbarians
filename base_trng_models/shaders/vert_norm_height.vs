#version 330 core
layout (location = 0) in vec3 a_Position;


out vec3 v_Position;
out vec3 ourColor;
//out vec3 ourColorView;
out vec2 TexCoord;
out mat3 TBN;


uniform mat4 draw;
uniform mat4 camera;
//uniform mat4 projection;
uniform mat4    u_BoneMatrices[128];

void main()
{
	vec3 a_Normal = vec3(0.0,0.0,1.0);
	vec2 texCoord = vec2(0.0,0.0);
	vec3 a_Tangent = vec3(0.0,1.0,0.0);
 	
	vec4 tv_Position = vec4(a_Position.xyz, 1.0);
	tv_Position = draw*(vec4(tv_Position.xyz, 1.0));
    v_Position = tv_Position.xyz;




        ourColor = vec3(draw* vec4(a_Normal, 0.0));
        


	
        vec3 tangent = vec3(draw* vec4(a_Tangent.xyz, 0.0));
		tangent = normalize(tangent - dot(tangent, ourColor) * ourColor);
		vec3 B = cross(ourColor, tangent);

		TBN = mat3(tangent, B, ourColor);
		
        gl_Position = camera * vec4(v_Position,1.0);


    	TexCoord.x = texCoord.x;
		TexCoord.y = 1.0 - texCoord.y;
}
