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
#include "VertexBufferLayout.h"
#include "Window.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

glm::mat4 GetTranslationMatrix(float tx, float ty, float tz)
{
	// glm 은 row major matrix
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

	Window mainWindow{ 800,600 };
	mainWindow.Initialize();

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

	//yaw 값이 0일때는 front가 [1,0,0]이므로, yaw를 90으로 해서 초기 front가 [0,0,-1]이 되도록 함
	Camera camera{ glm::vec3{0.0f,0.0f,5.0f}, glm::vec3{0.0f,1.0f,0.0f}, -90.0f, 0.0f, 5.0f, 0.5f };

	//앞뒤 이동 효과를 올바로 보기 위해서는 perspective projection 행렬 필요
	float aspect = (float)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight();
	glm::mat4 proj = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
	// Shader
	Shader shader{ "res/shaders/Basic.shader" };
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
	shader.SetUniformMat4f("u_Projection", proj);
	shader.SetUniformMat4f("u_View", camera.calculateViewMatrix());

	Renderer renderer;

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();

	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	/* Loop until the user closes the window */
	while (!mainWindow.GetShouldClose())
	{
		float now = glfwGetTime(); //현재 시간
		deltaTime = now - lastTime; //소요 시간 = 현재 시간 - 이전 프레임 시간 
		lastTime = now;

		//poll event 부분은 유저 입력에 필요하므로 남겨둠
		glfwPollEvents();

		camera.KeyControl(mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		/* Render here */
		renderer.Clear();

		shader.Bind();
		shader.SetUniformMat4f("u_View", camera.calculateViewMatrix()); //카메라 변화에 따라 새로 계산된 view 행렬 셰이더에 전달

		renderer.Draw(va, ib, shader);
		/* Swap front and back buffers */
		mainWindow.SwapBuffers();

	}
	return 0;
}