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

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragSource;
};

//���̴� ���� �Ľ� �Լ�
static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos) //vertex ���̴� ����
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) //fragment ���̴� ����
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n'; //�ڵ带 stringstream�� ����
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); //���̴� ��ü ����(��������)
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); // ���̴��� �ҽ� �ڵ� ���
	glCompileShader(id); // id�� �ش��ϴ� ���̴� ������

	// Error Handling(������ ���̴� ���α׷����Ҷ� ���Ӵ�...)
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //���̴� ���α׷����κ��� ������ ���(log)�� ����
	if (result == GL_FALSE) //�����Ͽ� ������ ���
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //log�� ���̸� ����
		char* message = (char*)alloca(length * sizeof(char)); //stack�� �����Ҵ�
		glGetShaderInfoLog(id, length, &length, message); //���̸�ŭ log�� ����
		std::cout << "���̴� ������ ����! " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id); //������ ������ ��� ���̴� ����
		return 0;
	}

	return id;
}

//--------Shader ���α׷� ����, ������, ��ũ----------//
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragShader)
{
	unsigned int program = glCreateProgram(); //���̴� ���α׷� ��ü ����(int�� ����Ǵ� ���� id)
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	//�����ϵ� ���̴� �ڵ带 program�� �߰��ϰ� ��ũ
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//���̴� ���α׷��� ���������Ƿ� vs, fs ���� ���α׷��� ���̻� �ʿ� ����
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


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
			-0.5f,-0.5f, 0.0f, //0
			 0.5f,-0.5f, 0.0f, //1
			 0.5f, 0.5f, 0.0f, //2
			-0.5f, 0.5f, 0.0f  //3
		};

		unsigned int indices[] = {
			0, 1, 2, // t1
			2, 3, 0, // t2
		};

		/* VAO�� ���� + �������ؼ������ ���ÿ��Ѵ� */
		unsigned int vao;  // vertext array object
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		VertexBuffer vb{ position,4 * 3 * sizeof(float) };

		// �����͸� �ؼ��ϴ� ���
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,
			3, // �ϳ��� vertex�� ��� �����͸� �ѱ����
			GL_FLOAT,
			GL_FALSE,
			sizeof(float) * 3,
			0);

		IndexBuffer ib{ indices,6 };

		// Shader
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

		unsigned int shader = CreateShader(source.VertexSource, source.FragSource);
		GLCall(glUseProgram(shader)); //BindBuffer�� ����������, ���� ���̴� ���α׷��� "�۾� ����"�� ����
		//draw call�� �۾� ������ ���̴� ���α׷��� ����Ͽ� �۾� ������ ���� �����͸� �׸�

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

		// 0�������� Unbind��
		glBindVertexArray(0); // vao
		glBindBuffer(GL_ARRAY_BUFFER, 0); // buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // ibo
		glUseProgram(0); // shader

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			//glUseProgram(0); // deactivate
			//glDrawArrays(GL_TRIANGLES, 0, 3); // draw call

			// VAO ��� ���ε�
			glBindVertexArray(vao);

			// shader ���
			glUseProgram(shader); // shade

			GLCall(glDrawElements(GL_TRIANGLES,
				6,
				GL_UNSIGNED_INT,
				nullptr));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glDeleteProgram(shader);
	}
	
	glfwTerminate();
	return 0;
}