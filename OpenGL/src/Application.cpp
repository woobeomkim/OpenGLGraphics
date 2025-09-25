//http://glew.sourceforge.net/
//사이트에서 GLEW 바이너리 버전다운로드. 필요 라이브러리 dependencies에 복사 후 설정
//http://glew.sourceforge.net/basic.html
//아래 예제 코드실행 확인 

//GLEW_STATIC Define 필요
#include <GL/glew.h> //glfw보다 먼저 include해야 함
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

// 이번에는 Vertex Array와 Shader를 이용하여 삼각형을 그리는 Modern OpenGL 방식으로 구현할 것임
// Vertex Array는 GPU의 VRAM에 Buffer에 저장되는 데이터를 넘기는 방식을 이야기함
// 데이터를 넘기고 나서 삼각형을 실제로 그리는 명령을 호출하는 것을 Draw call이라고 함
// 삼각형이 어떻게 그려질지 사용자가 구현한 프로그램이 Shader임

// OpenGL은 State Machine과 비슷함. 일반적인 객체 지향 프로그램의 설계와는 다르게,
// 현재 처리할 데이터를 선택한 후, 현재 실행해야 할 작업을 함수 호출을 통해 처리하는 방식임
// 간단하게 두 삼각형을 화면에 그리는 의사코드로 설명하면,

// ---객체지향 방식
// Triangle t1, t2; //삼각형 두 개를 정의
// Draw(t1); //t1 객체를 전달함으로써 삼각형 1을 그림
// Draw(t2); //t2 객체를 전달함으로써 삼각형 2를 그림

// ---State Machine 방식
// Triangle t1, t2; //삼각형 두 개를 정의
// Activate(t1); //삼각형 1을 처리중인 상태로 설정
// Draw(); //현재 처리중인 데이터(=삼각형 1)를 화면에 그림
// Activate(t2); //삼각형 2를 처리중인 상태로 설정
// Draw(); //현재 처리중인 데이터(=삼각형 2)를 화면에 그림

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Opengl 3.3 버전 사용
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); //Compatability 버전일때는 VAO를 안만들어도 동작
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// glfwMakeContextCurrent가 호출된 후에 glewInit이 수행되어야 함
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	std::cout << glGetString(GL_VERSION) << std::endl; //내 플랫폼의 GL_Version 출력해보기
	
	// 소멸자 확인용 스코프
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

			// shader 사용
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