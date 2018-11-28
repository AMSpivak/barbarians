#version 330 core
layout (location = 0) in vec3 a_Position;


out vec3 v_Position;
out vec3 ourColor;
//out vec3 ourColorView;
out vec2 TexCoord;
//out mat3 TBN;
//


uniform vec3 offset_position;
uniform vec4 map_position;
uniform mat4 camera;
//uniform mat4 projection;
//uniform mat4    u_BoneMatrices[128];

uniform sampler2D HeightMap;


void main()
{
	vec3 position = a_Position.xyz+ offset_position;



	vec2 texCoord = vec2(map_position.x + position.x,map_position.y + position.z) * map_position.z + vec2(0.5,0.5);
	vec4 texColor = texture(HeightMap, texCoord);
	float multiplier = map_position.z * 0.15f;
	vec4 texColor_x1 = texture(HeightMap , texCoord + vec2(1.0,0.0)* multiplier);
	vec4 texColor_x2 = texture(HeightMap , texCoord + vec2(-1.0,0.0)* multiplier);
	vec4 texColor_y1 = texture(HeightMap , texCoord + vec2(0.0,1.0)* multiplier);
	vec4 texColor_y2 = texture(HeightMap , texCoord + vec2(0.0,-1.0)* multiplier);

	position.y += texColor.x * map_position.w;
    v_Position = position.xyz;

	//vec3 a_Tangent = normalize(vec3(2.0, texColor_x1.x-texColor_x2.x, 2.0))
	//vec3 B = normalize(vec3(2.0, texColor_y1.x-texColor_y2.x, 2.0))
	//vec3 a_Normal = cross(a_Tangent, B)
	vec3 Normal = normalize(vec3(-texColor_x1.x+texColor_x2.x, 0.5/map_position.w, -texColor_y1.x+texColor_y2.x));

 	





        ourColor = vec3( vec4(Normal, 0.0));
        


	
        //vec3 tangent = vec3(vec4(a_Tangent.xyz, 0.0));
		//tangent = normalize(tangent - dot(tangent, ourColor) * ourColor);
		//vec3 B = cross(ourColor, tangent);

		//TBN = mat3(tangent, B, ourColor);
		
        gl_Position = camera * vec4(v_Position,1.0);


    	TexCoord.x = texCoord.x;
		TexCoord.y = 1.0 - texCoord.y;
}
