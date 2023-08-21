#version 400
in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 projection;

void main()
{
    gl_Position = projection*View*Model*vPosition/vPosition.w;
	color=vColor;
}//