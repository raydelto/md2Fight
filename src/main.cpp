#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Md2.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

bool maxm;
float yValue = 0.0f;
float yInc = 0.0f;
bool jumping = false;
bool jumpingUp = true;
bool crowching = false;
float rot = 0.0f;
float rotInc = 0.0f;
float angle, run, transL;
float xIncrement = 0.0;
float zTrans = 0.0;
float xPlayer1 = -69.0;
float ambientlight[] = {0.5, 0.5, 0.5, 1.0};
float diffuselight[] = {0.9, 0.9, 0.9, 1.0};
float LightPos[] = {0.0, 0.0, 0.0, 1.0};
float matspec[] = {1.0, 1.0, 0.0, 1.0};
float jumpY = 0.00;
Md2 player4((char *)"data/female.md2", (char *)"data/female.tga");
int zoom = 0;
GLuint pa;
GLuint sand;
const char* APP_TITLE = "MD2 Fight v0.2";
GLFWwindow *window;

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
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
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch(action)
	{
	case GLFW_PRESS:
		switch(key)
		{

			case GLFW_KEY_L:
				jumping = true;
			break;
			case GLFW_KEY_K:
				crowching = true;
			break;

			case GLFW_KEY_UP:
				yInc = 1.0f;
				
			break;
			case GLFW_KEY_DOWN:
				yInc = -1.0f;
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
			case GLFW_KEY_B:
				rotInc = 1.0f;
			break;
			case GLFW_KEY_V:
				rotInc = -1.0f;
			break;
			case GLFW_KEY_N:
				rotInc = 0.0f;
			break;


		}
	break;

	case GLFW_RELEASE:
		xIncrement = 0.0;
		switch(key)
		{
			case GLFW_KEY_K:
				crowching = false;
			break;
			case GLFW_KEY_V:
			case GLFW_KEY_B:
				rotInc = 0.0f;
			break;
			case GLFW_KEY_DOWN:
			case GLFW_KEY_UP:
				yInc = 0.0f;
			break;
		}

	break;
	}
}


void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void init(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	perspectiveGL(90.0, 1.0, 2.0, 20000);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientlight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuselight);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);

	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
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
	glClearColor(0.0, 1.0, 0.0, 0.0);


	if (transL >= 40)
	{
		maxm = true;
	}
	else if (transL <= 10)
	{
		maxm = false;
	}

	xPlayer1 += xIncrement;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	yValue += yInc;
	glTranslatef(xPlayer1, yValue, -100 + zTrans);
	
	rot += rotInc;
	glRotatef(rot , 0.0, 1.0, 0.0);

	if (crowching)
	{
		player4.Draw(113, 122);
	}
	else
	{
		player4.Draw(0, 39);
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glFlush();
}

bool initGlfw()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);


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
	if (!initGlfw())
	{
		printf("Unable to initialize GLFW\n");
		return 1;
	}
	int done;

	init();

	bool init = false;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		std::this_thread::sleep_for (std::chrono::milliseconds(10));

		draw();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
		if(!init)
		{
			int x,y;
			glfwGetWindowPos(window,&x,&y);
			glfwSetWindowPos(window, x+1,y);
			init = true;
		}
	}
	return 0;
}
