#pragma once

#include <GL/glew.h> //glfw���� ���� include�ؾ� ��
#include "VertexArray.h"
#include "IndexBuffer.h"

class Shader;

//__debugbreak()�� MSVC���� ��� ����
#define ASSERT(x) if ((!(x))) __debugbreak(); 
#define GLCall(x) GLClearError();\
				  x;\
				  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//glGetError�� ������ �ϳ����� ��ȯ�ϱ� ������, �� �� Ȯ�ο� ��� ������ �̾Ƴ��� ���� �ʿ���
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	//ȭ�鿡 ���𰡸� �׸����ؼ��� Vertex Array (Vertex Buffer�� ���⿡ ���ԵǾ� ����), Index Array, Shader Program�� �ʿ���
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
};