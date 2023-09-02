#vertex2d
#version 330 core
layout(location = 0) in vec4 position;

uniform float u_Zoom;
uniform vec2 u_Offset;

void main(void)
{
	gl_Position.x = position.x * u_Zoom + u_Offset.x;
	gl_Position.y = position.y * u_Zoom + u_Offset.y;
}
#vertex3d
#version 330 core
layout(location = 0) in vec4 position;

uniform float u_Zoom;
uniform float u_Height;
uniform float u_Width;
uniform mat4 u_RotationMat;

void main(void)
{
	vec4 p = u_RotationMat * position;
	gl_Position.x = (p.x / u_Width) * u_Zoom;
	gl_Position.y = (p.y / u_Height) * u_Zoom;
}
#fragment
#version 330 core

out vec4 color;

uniform vec4 u_Color;

void main(void)
{
	color = u_Color;
}