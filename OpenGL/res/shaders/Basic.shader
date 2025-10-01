#shader vertex
#version 330 core

layout(location = 0) in vec4 position; 
//layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform vec4 u_Color; //uniform에는 id가 할당됨

void main()
{
	gl_Position = u_Projection * u_View * position;  // gl_Position에 넣어야 할 정보 : Clip space coordinate
	//v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord; //버텍스 셰이더에서 넘겨받은 데이터

uniform sampler2D u_Texture; //texture는 sampler2D 타입

void main()
{
	//vec4 texColor = texture(u_Texture, v_TexCoord); //texture 함수는 texcoord위치의 픽셀 색상 값을 정의된 방법에 따라 샘플링
	//color = texColor; //이제 텍스처 이미지에서 샘플링한 색상을 fragment 색상으로 결정
	color = vec4(1.0f,0.0f,0.0f,1.0f);
};