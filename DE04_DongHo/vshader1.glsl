#version 400
in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = model*vPosition;
	color=vColor;
}//