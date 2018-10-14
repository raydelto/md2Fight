#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "md2Class.cpp"

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
double elapsed;
float jumpY = 0.00;
md2 player((char *)"data/tris.md2", (char *)"data/skin.tga");
md2 player2((char *)"data/cyborg.md2", (char *)"data/cyborg1.tga");
md2 player3((char *)"data/grunt.md2", (char *)"data/grunt.tga");
md2 player4((char *)"data/female.md2", (char *)"data/female.tga");
int zoom = 0;
double start, end;
GLuint pa;
GLuint sand;
tga pa_d((char *)"data/lab.tga");
tga sa((char *)"data/sand2.tga");
const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Colored Triangle";
GLFWwindow *window;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void init(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 1.0, 2.0, 20000);
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

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &pa);
	glBindTexture(GL_TEXTURE_2D, pa);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pa_d.width, pa_d.height, GL_RGB, GL_UNSIGNED_BYTE, pa_d.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &sand);
	glBindTexture(GL_TEXTURE_2D, sand);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sa.width, sa.height, GL_RGB, GL_UNSIGNED_BYTE, sa.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glPushMatrix();

	glTranslatef(0.0, -50.0, 0.0 + zTrans);

	glPushMatrix();
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

	glTranslatef(-50 + (transL * 3), 0.0, -150 - (50 * sqrt(1 - (sin(transL) * sin(transL)))) + zTrans);
	glRotatef(angle, 0.0, 1.0, 0.0);
	player.Draw(40, 46);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	xPlayer1 += xIncrement;
	if (xPlayer1 >= 25.00)
		xPlayer1 = 24.99;
	glTranslatef(xPlayer1, jump(), -100 + zTrans);

	if (jumping)
	{
		player2.Draw(67, 73);
	}
	else if (crowching)
	{
		player2.Draw(136, 154);
	}
	else
	{
		player2.Draw(40, 46);
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(-25, 0.0, -150);
	player3.Draw(40, 46);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(xPlayer1 + 50, 0.0, -100 + zTrans);

	if (crowching)
	{
		player4.Draw(113, 122);
	}
	else
	{
		player4.Draw(40, 46);
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sand);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 10.0 - run);
		glVertex3f(-1000, -25.0, -1000);
		glTexCoord2f(10.0, 10.0 - run);
		glVertex3f(-1000, -25.0, 1000);
		glTexCoord2f(10.0, 0.0 - run);
		glVertex3f(1000, -25.0, 1000);
		glTexCoord2f(0.0, 0.0 - run);
		glVertex3f(1000, -25.0, -1000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pa);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0 + run, 0.0);
		glVertex3f(-1000, -25.0, -1000);
		glTexCoord2f(1.0 + run, 0.0);
		glVertex3f(1000, -25.0, -1000);
		glTexCoord2f(1.0 + run, 1.0);
		glVertex3f(1000, 1100.0, -1000);
		glTexCoord2f(0.0 + run, 1.0);
		glVertex3f(-1000, 1100.0, -1000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
	glFlush();
	SDL_GL_SwapBuffers();
}

bool glfwInit()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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

int main(void)
{
	int done;

	if (!glfwInit())
	{
		printf("Unable to initialize GLFW");
		return 1;
	}

	init();
	done = 0;
	start = glfwGetTime();

	while (!done)
	{
		if (elapsed >= 0.0083)
		{
			start = glfwGetTime();
			draw();
		}
		end = glfwGetTime();
		elapsed = end - start;
	}
	return 0;
}
