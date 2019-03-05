#version 330 core
layout (location = 0) in vec3 a_Position;


out vec3 v_Position;
out vec3 ourColor;
//out vec3 ourColorView;
out vec2 TexCoord;
//out mat3 TBN;
//


uniform vec4 offset_position;
uniform vec4 map_position;
uniform mat4 camera;
//uniform mat4 projection;
//uniform mat4    u_BoneMatrices[128];

uniform sampler2D HeightMap;

float Triangular( float f )
{
	f = f / 2.0;
	if( f < 0.0 )
	{
		return ( f + 1.0 );
	}
	else
	{
		return ( 1.0 - f );
	}
	return 0.0;
}

vec4 BiCubic( sampler2D textureSampler, vec2 TexCoord, vec2 TextureSize, vec2 TexelSize )
{
    //float texelSizeX = 1.0 / fWidth; //size of one texel 
    //float texelSizeY = 1.0 / fHeight; //size of one texel 
    vec4 nSum = vec4( 0.0, 0.0, 0.0, 0.0 );
    vec4 nDenom = vec4( 0.0, 0.0, 0.0, 0.0 );
    float a = fract( TexCoord.x * TextureSize.x ); // get the decimal part
    float b = fract( TexCoord.y * TextureSize.y ); // get the decimal part
	vec3 probes[16];
	int probe_position = 0;
    for( int m = -1; m <=2; m++ )
    {
        for( int n =-1; n<= 2; n++)
        {
			vec4 vecData = texture2D(textureSampler, 
                               TexCoord + vec2(TexelSize.x * float( m ), 
					TexelSize.y * float( n )));

			float f  = Triangular( float( m ) - a );
			vec4 vecCooef1 = vec4( f,f,f,f );
			float f1 = Triangular ( -( float( n ) - b ) );
			vec4 vecCoeef2 = vec4( f1, f1, f1, f1 );
            nSum = nSum + ( vecData * vecCoeef2 * vecCooef1  );
            nDenom = nDenom + (( vecCoeef2 * vecCooef1 ));
			probes[probe_position] = vec3(vecData.x,f,f1);
			//probes[probe_position] = vec3(vecData.x,1.0,1.0);
			probe_position++;
        }
		//probe_position++;
    }
	vec4 h = nSum / nDenom;
	float gz = (probes[7].x - h.x)*probes[7].y - (probes[4].x - h.x)*probes[4].y;
	float gx = (probes[14].x - h.x)*probes[14].z - (probes[2].x - h.x)*probes[2].z;
	ourColor = normalize(vec3(-gx,0.01,-gz));
    return h;
}

vec4 tex2DBiLinear( sampler2D textureSampler_i, vec2 texCoord_i, vec2 TextureSize, vec2 TexelSize )
{
    vec4 p0q0 = texture2D(textureSampler_i, texCoord_i);
    vec4 p1q0 = texture2D(textureSampler_i, texCoord_i + vec2(TexelSize.x, 0));

    vec4 p0q1 = texture2D(textureSampler_i, texCoord_i + vec2(0, TexelSize.y));
    vec4 p1q1 = texture2D(textureSampler_i, texCoord_i + vec2(TexelSize.x , TexelSize.y));

    float a = fract( texCoord_i.x * TextureSize.x ); // Get Interpolation factor for X direction.
					// Fraction near to valid data.

    vec4 pInterp_q0 = mix( p0q0, p1q0, a ); // Interpolates top row in X direction.
    vec4 pInterp_q1 = mix( p0q1, p1q1, a ); // Interpolates bottom row in X direction.

    float b = fract( texCoord_i.y * TextureSize.y );// Get Interpolation factor for Y direction.
    return mix( pInterp_q0, pInterp_q1, b ); // Interpolate in Y direction.
}

void main()
{
	vec3 position = a_Position.xyz + offset_position.xyz;



	vec2 texCoord = vec2(map_position.x + position.x,map_position.y + position.z) * map_position.z + vec2(0.5,0.5);
	vec2 textureSize = textureSize(HeightMap, 0);
	vec2 texelSize = 1.0 / textureSize;
	vec4 texColor = texture(HeightMap , texCoord);//BiCubic(HeightMap, texCoord,textureSize,texelSize);
	float multiplier = texelSize.x;//map_position.z * offset_position.w;
	float height_x1 = texture(HeightMap , texCoord + vec2(1.0,0.0)* multiplier).x;
	float height_x2 = texture(HeightMap , texCoord + vec2(-1.0,0.0)* multiplier).x;
	float height_y1 = texture(HeightMap , texCoord + vec2(0.0,1.0)* multiplier).x;
	float height_y2 = texture(HeightMap , texCoord + vec2(0.0,-1.0)* multiplier).x;
	float height = texColor.x;

	float gz = map_position.w *(height_y2 - height_y1);
	float gx = map_position.w *(height_x2 - height_x1);
	ourColor = normalize(vec3(-gx,2.0,-gz));

	//position.y += 0.1f;//height * map_position.w;
	position.y += height * map_position.w;
    v_Position = position.xyz;

	//vec3 X = vec3(1.0 * offset_position.w, (height_x2 - height_x1)* map_position.w, 0.0);
	//vec3 Z = vec3(0.0, -(height_y2 - height_y1)* map_position.w, 1.0 * offset_position.w);
	//vec3 Normal = normalize(cross(Z, X));
	//vec3 Normal = vec3(texColor.x,texColor.x,texColor.x);

    //ourColor = vec3( vec4(Normal, 0.0));

        //vec3 tangent = vec3(vec4(a_Tangent.xyz, 0.0));
		//tangent = normalize(tangent - dot(tangent, ourColor) * ourColor);
		//vec3 B = cross(ourColor, tangent);

		//TBN = mat3(tangent, B, ourColor);
		
    gl_Position = camera * vec4(v_Position,1.0);

    TexCoord.x = texCoord.x;
	TexCoord.y = 1.0 - texCoord.y;
}
