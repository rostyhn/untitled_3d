#version 330 core
layout (location = 0) in vec3 vertex; 
layout (location = 1) in vec2 texture;

out vec2 TexCoords;

uniform int isBillboard;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	TexCoords = texture.xy;
	vec4 finalPos = vec4(1.0,1.0,1.0,1.0);
	if(isBillboard == 0)
	{
		finalPos = projection * vec4(vertex.xy, 0.0, 1.0);
	}
	else
	{
		finalPos = projection * model * view * vec4(vertex.xy,0.0,1.0);
	}
	gl_Position = finalPos;
}  