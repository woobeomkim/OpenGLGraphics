#pragma once

#include <GL/glew.h> //glfw보다 먼저 include해야 함
#include "VertexArray.h"
#include "IndexBuffer.h"

class Shader;

//__debugbreak()는 MSVC에만 사용 가능
#define ASSERT(x) if ((!(x))) __debugbreak(); 
#define GLCall(x) GLClearError();\
				  x;\
				  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//glGetError는 에러를 하나씩만 반환하기 때문에, 한 번 확인에 모든 오류를 뽑아내는 것이 필요함
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	//화면에 무언가를 그리위해서는 Vertex Array (Vertex Buffer는 여기에 포함되어 있음), Index Array, Shader Program이 필요함
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Clear() const;
};