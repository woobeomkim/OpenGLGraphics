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

// �̹����� Vertex Array�� Shader�� �̿��Ͽ� �ﰢ���� �׸��� Modern OpenGL ������� ������ ����
// Vertex Array�� GPU�� VRAM�� Buffer�� ����Ǵ� �����͸� �ѱ�� ����� �̾߱���
// �����͸� �ѱ�� ���� �ﰢ���� ������ �׸��� ����� ȣ���ϴ� ���� Draw call�̶�� ��
// �ﰢ���� ��� �׷����� ����ڰ� ������ ���α׷��� Shader��

// OpenGL�� State Machine�� �����. �Ϲ����� ��ü ���� ���α׷��� ����ʹ� �ٸ���,
// ���� ó���� �����͸� ������ ��, ���� �����ؾ� �� �۾��� �Լ� ȣ���� ���� ó���ϴ� �����
// �����ϰ� �� �ﰢ���� ȭ�鿡 �׸��� �ǻ��ڵ�� �����ϸ�,

// ---��ü���� ���
// Triangle t1, t2; //�ﰢ�� �� ���� ����
// Draw(t1); //t1 ��ü�� ���������ν� �ﰢ�� 1�� �׸�
// Draw(t2); //t2 ��ü�� ���������ν� �ﰢ�� 2�� �׸�

// ---State Machine ���
// Triangle t1, t2; //�ﰢ�� �� ���� ����
// Activate(t1); //�ﰢ�� 1�� ó������ ���·� ����
// Draw(); //���� ó������ ������(=�ﰢ�� 1)�� ȭ�鿡 �׸�
// Activate(t2); //�ﰢ�� 2�� ó������ ���·� ����
// Draw(); //���� ó������ ������(=�ﰢ�� 2)�� ȭ�鿡 �׸�

int main(void)
{
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
	
	// �Ҹ��� Ȯ�ο� ������
	{
		glEnable(GL_CULL_FACE);
		float position[] = {
			-0.5f,-0.5f, 0.0f, 1.0f, 0.0f,0.0f, //0
			 0.5f,-0.5f, 0.0f, 0.0f, 1.0f,0.0f, //1
			 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,1.0f, //2
			-0.5f, 0.5f, 0.0f, 1.0f, 0.0f,1.0f,  //3
		};

		unsigned int indices[] = {
			0, 1, 2, // t1
			2, 3, 0, // t2
		};

		VertexArray va;
		VertexBuffer vb{ position, 4 * 7 * sizeof(float) };

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		va.AddBuffer(vb, layout);

		IndexBuffer ib{ indices,6 };

		// Shader
		Shader shader{ "res/shaders/Basic.shader" };
		shader.Bind();


		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			//glUseProgram(0); // deactivate
			//glDrawArrays(GL_TRIANGLES, 0, 3); // draw call

			va.Bind();

			// shader ���
			shader.Bind();

			GLCall(glDrawElements(GL_TRIANGLES,
				6,
				GL_UNSIGNED_INT,
				nullptr));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}
	
	glfwTerminate();
	return 0;
}