#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <thread>
#include <chrono>

#include "Md2.h"
#include "ShaderProgram.h"
#include "Camera.h"

using namespace std;

bool maxm;
bool jumping = false;
bool jumpingUp = true;
bool crowching = false;
float angle, run, transL;
float xIncrement = 0.0;
float zTrans = 0.0;
float xPlayer1 = -69.0;
float ambientlight[] = {0.5, 0.5, 0.5, 1.0};
float diffuselight[] = {0.9, 0.9, 0.9, 1.0};
float LightPos[] = {0.0, 0.0, 0.0, 1.0};
float matspec[] = {1.0, 1.0, 0.0, 1.0};
float jumpY = 0.00;
Md2* player;
int zoom = 0;
GLuint pa;
GLuint sand;
Tga pa_d((char *)"data/lab.tga");
Tga sa((char *)"data/sand2.tga");
const char *APP_TITLE = "MD2 Fight v0.2";
GLFWwindow *window;
FPSCamera fpsCamera(glm::vec3(0.0f, 3.0f, 10.0f));

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Function prototypes
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	switch (action)
	{
	case GLFW_PRESS:
		switch (key)
		{
		case GLFW_KEY_UP:
			jumping = true;
			break;
		case GLFW_KEY_DOWN:
			crowching = true;
			break;
		case GLFW_KEY_RIGHT:
			xIncrement = 0.5;
			break;
		case GLFW_KEY_LEFT:
			xIncrement = -0.5;
			break;
		case GLFW_KEY_H:
			zoom = 1;
			break;
		case GLFW_KEY_G:
			zoom = -1;
			break;
		case GLFW_KEY_J:
			zoom = 0;
			break;
		}
		break;

	case GLFW_RELEASE:
		xIncrement = 0.0;
		switch (key)
		{
		case GLFW_KEY_DOWN:
			crowching = false;
			break;
		}

		break;
	}
}

float jump(void)
{
	if (jumping)
	{
		if (jumpingUp)
		{
			jumpY += 0.5;

			if (jumpY >= 16)
			{
				jumpingUp = false;
			}
		}
		else
		{
			jumpY -= 0.5;
			if (jumpY <= 0.00)
			{
				jumping = false;
				jumpingUp = true;
			}
		}
	}

	return jumpY;
}
void draw(void)
{

	if (zoom == 1)
	{

		zTrans += 1.2;
	}

	else if (zoom == -1)
	{

		zTrans -= 1.2;
	}

	/*
0-39 idle
40-46 running
47-60 shot, not falling
61-66 shot in shoulder
67-73 jump
75-95 idle
96-112 shot falling down
113-122 idle
123-135 idle
136-154 crouching
155-161 crouchin crawl
162-169 idle crouching
170-177 kneeling dying
178-185 falling back dying
186-190 falling forward dying
191-198 falling back slowly dying
 *
 *
 */

	if (!crowching)
		run += 0.01;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	if (transL >= 40)
	{
		maxm = true;
	}
	else if (transL <= 10)
	{
		maxm = false;
	}

	angle = 50 * atan((sin(run) * cos(run)) / sqrt((1 - (sin(run) * sin(run)))));
	if (maxm)
	{
		transL -= 0.01;
	}
	else
	{
		transL += 0.01;
		angle = -angle;
	}

	player->Draw(40, 46);

	// if (jumping)
	// {
	// 	player2->Draw(67, 73);
	// }
	// else if (crowching)
	// {
	// 	player2->Draw(136, 154);
	// }
	// else
	// {
	// 	player2->Draw(40, 46);
	// }

	// player3->Draw(40, 46);

	// if (crowching)
	// {
	// 	player4->Draw(113, 122);
	// }
	// else
	// {
	// 	player4->Draw(40, 46);
	// }
}

void initOpenGl()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	player = new Md2((char *)"data/tris.md2", (char *)"data/skin.tga");
	// player2 = new Md2((char *)"data/cyborg.md2", (char *)"data/cyborg1.tga");
	// player3 = new Md2((char *)"data/grunt.md2", (char *)"data/grunt.tga");
	// player4 = new Md2((char *)"data/female.md2", (char *)"data/female.tga");	
}

bool initGlfw()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // forward compatible with newer versions of OpenGL as mody become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, APP_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, glfw_onKey);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	return true;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(void)
{
	cout << "DALE" << endl;
	if (!initGlfw())
	{
		printf("Unable to initialize GLFW\n");
		return 1;
	}
	initOpenGl();
	int done;

	bool init = false;
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");
	

	glm::mat4 model, view, projection;


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

				// Create the View matrix
		view = fpsCamera.getViewMatrix();

		// Create the projection matrix
		projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
		shaderProgram.use();
		// Pass the matrices to the shader
		shaderProgram.setUniform("view", view);
		shaderProgram.setUniform("projection", projection);

		model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -6.0f)) * glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
		shaderProgram.setUniform("model", model);		


		draw();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (!init)
		{
			int x, y;
			glfwGetWindowPos(window, &x, &y);
			glfwSetWindowPos(window, x + 1, y);
			init = true;
		}
	}
	return 0;
}
