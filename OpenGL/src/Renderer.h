#pragma once

#include <GL/glew.h> //glfw보다 먼저 include해야 함

//__debugbreak()는 MSVC에만 사용 가능
#define ASSERT(x) if ((!(x))) __debugbreak(); 
#define GLCall(x) GLClearError();\
				  x;\
				  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//glGetError는 에러를 하나씩만 반환하기 때문에, 한 번 확인에 모든 오류를 뽑아내는 것이 필요함
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);