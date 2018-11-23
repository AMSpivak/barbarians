#version 330 core
layout (location = 0) in vec3 a_Position;


out vec3 v_Position;
out vec3 ourColor;
//out vec3 ourColorView;
out vec2 TexCoord;
out mat3 TBN;



uniform mat4 camera;


void main()
{
 	
	vec4 tv_Position = vec4(a_Position.xyz, 1.0);
    v_Position = tv_Position.xyz;

	ourColor = vec3(0.0, 1.0, 0.0);

	vec3 tangent = vec3(1.0, 0.0, 0.0);

	tangent = normalize(tangent - dot(tangent, ourColor) * ourColor);

	vec3 B = cross(ourColor, tangent);

	TBN = mat3(tangent, B, ourColor);
	
	gl_Position = camera * vec4(v_Position,1.0);


	TexCoord.x = a_Position.x;
	TexCoord.y = 1.0 - a_Position.z;
}
