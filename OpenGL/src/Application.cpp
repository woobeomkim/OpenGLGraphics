//http://glew.sourceforge.net/
//����Ʈ���� GLEW ���̳ʸ� �����ٿ�ε�. �ʿ� ���̺귯�� dependencies�� ���� �� ����
//http://glew.sourceforge.net/basic.html
//�Ʒ� ���� �ڵ���� Ȯ�� 

//GLEW_STATIC Define �ʿ�
#include <GL/glew.h> //glfw���� ���� include�ؾ� ��
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

glm::mat4 GetTranslationMatrix(float tx, float ty, float tz)
{
	// glm �� row major matrix
	glm::mat4 T{ 1.0f,0.0f,0.0f,tx,
				 0.0f,1.0f,0.0f,ty,
				 0.0f,0.0f,1.0f,tz,
				 0.0f,0.0f,0.0f,1.0f };

	return glm::transpose(T);
}

glm::mat4 GetProjectionMatrix(float fovy, float aspect, float n, float f)
{
	float halfCot = 1 / (glm::tan(fovy / 2));
	glm::mat4 P{ halfCot / aspect,0.0f,0.0f,0.0f,
				 0.0f,halfCot,0.0f,0.0f,
				 0.0f,0.0f,-(f + n) / (f - n), -2 * n * f / (f - n),
				 0.0f,0.0f,-1.0f,0.0f };

	return glm::transpose(P);
}

int main(void)
{
	glm::mat4 T = GetTranslationMatrix(1.0f, 0.0f, 0.0f);
	glm::vec4 pos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::vec4 translatedPosition = T * pos;

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Opengl 3.3 ���� ���
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //Compatability �����϶��� VAO�� �ȸ��� ����
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// glfwMakeContextCurrent�� ȣ��� �Ŀ� glewInit�� ����Ǿ�� ��
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	std::cout << glGetString(GL_VERSION) << std::endl; //�� �÷����� GL_Version ����غ���
	

	glEnable(GL_CULL_FACE);
	float position[] = {
		-0.5f,-0.5f, -5.0f, //0
		 0.5f,-0.5f, -5.0f, //1
		 0.5f, 0.5f, -5.0f, //2
		-0.5f, 0.5f, -5.0f,  //3
	};

	unsigned int indices[] = {
		0, 1, 2, // t1
		2, 3, 0, // t2
	};

	VertexArray va;
	VertexBuffer vb{ position, 3 * 4 * sizeof(float) };

	VertexBufferLayout layout;
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);

	IndexBuffer ib{ indices,6 };

	// modle view projection matrix ����
	glm::mat4 model = GetTranslationMatrix(0.0f, 0.0f, 0.0f);
	glm::mat4 proj = GetProjectionMatrix(3.14F / 3, 640 / 480 , 1.0f, 100.0f);

	//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

	// Shader
	Shader shader{ "res/shaders/Basic.shader" };
	shader.Bind();

	Renderer renderer;

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		renderer.Clear();

		shader.Bind();
		shader.SetUniformMat4f("u_Model", model);
		shader.SetUniformMat4f("u_Proj", proj);
		
		renderer.Draw(va, ib, shader);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}