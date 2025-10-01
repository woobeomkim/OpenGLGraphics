#shader vertex
#version 330 core

layout(location = 0) in vec4 position; 
//layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec4 u_Color; //uniform���� id�� �Ҵ��

void main()
{
	gl_Position = u_Projection * u_View * position;  // gl_Position�� �־�� �� ���� : Clip space coordinate
	//v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord; //���ؽ� ���̴����� �Ѱܹ��� ������

uniform sampler2D u_Texture; //texture�� sampler2D Ÿ��

void main()
{
	//vec4 texColor = texture(u_Texture, v_TexCoord); //texture �Լ��� texcoord��ġ�� �ȼ� ���� ���� ���ǵ� ����� ���� ���ø�
	//color = texColor; //���� �ؽ�ó �̹������� ���ø��� ������ fragment �������� ����
	color = vec4(1.0f,0.0f,0.0f,1.0f);
};