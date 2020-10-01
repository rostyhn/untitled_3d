#version 330 core
layout (location = 0) in vec3 vertex; 
layout (location = 1) in vec2 texture;

out vec2 TexCoords;

uniform int isBillboard;
uniform mat4 projection;
uniform vec3 world_pos;
uniform mat4 view;

void main()
{
	TexCoords = texture.xy;
	vec4 finalPos = vec4(1.0,1.0,1.0,1.0);
	if(isBillboard == 0)
	{
		finalPos = projection * vec4(vertex.xyz, 1.0);
	}
	else
	{
		vec3 cr = vec3(view[0][0], view[1][0], view[2][0]);
		vec3 cu = vec3(view[0][1], view[1][1], view[2][1]);
	
		vec3 correctPos = world_pos + cr * vertex.x + cu * vertex.y;
		finalPos = projection * view * vec4(correctPos.xyz, 1.0);
	}
	gl_Position = finalPos;
}  