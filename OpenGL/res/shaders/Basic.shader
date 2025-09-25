#shader vertex
#version 330 core

layout(location = 0) in vec4 position; 

uniform mat4 u_Model;
uniform mat4 u_Proj;

void main()
{
	gl_Position = u_Proj * u_Model * position;  // gl_Position에 넣어야 할 정보 : Clip space coordinate

};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(1.0f,0.0f,0.0f,1.0f); 
};