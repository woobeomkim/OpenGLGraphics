#pragma once

#include <GL/glew.h> //glfw���� ���� include�ؾ� ��

//__debugbreak()�� MSVC���� ��� ����
#define ASSERT(x) if ((!(x))) __debugbreak(); 
#define GLCall(x) GLClearError();\
				  x;\
				  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//glGetError�� ������ �ϳ����� ��ȯ�ϱ� ������, �� �� Ȯ�ο� ��� ������ �̾Ƴ��� ���� �ʿ���
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);