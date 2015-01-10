#include <SDL/SDL_mixer.h>
#include "md2Class.cpp"

md2 player((char*)"data/tris.md2", (char*)"data/skin.tga");
md2 player2((char*)"data/cyborg.md2", (char*)"data/cyborg1.tga");
md2 player3((char*)"data/grunt.md2", (char*)"data/grunt.tga");
md2 player4((char*)"data/female.md2", (char*)"data/female.tga");
float xIncrement=0.0;
float zTrans=0.0;
int zoom=0;
double elapsed;
bool jumping=false;
bool jumpingUp=true;
bool crowching=false;
double jumpY=0.00;
clock_t start, end;
SDL_Event event;
Mix_Music *music = NULL;
float xPlayer1=-69.0;
float ambientlight[] = {0.5, 0.5, 0.5, 1.0};
float diffuselight[] = {0.9, 0.9, 0.9, 1.0};
float LightPos[] = {0.0, 0.0, 0.0, 1.0};
float matspec[] = {1.0, 1.0, 0.0, 1.0};

GLuint pa;
GLuint sand;

tga pa_d((char*)"data/lab.tga");
tga sa((char*)"data/sand2.tga");



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
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);				
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &sand);
	glBindTexture(GL_TEXTURE_2D, sand);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sa.width, sa.height, GL_RGB, GL_UNSIGNED_BYTE, sa.data);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);				
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	
}

float run,transL;

bool maxm;

float angle;



double jump(void)
 {

    if (jumping)
      {
    	   if (jumpingUp)
	   {
	     jumpY+=0.5;
	    
	     if (jumpY>=16)
	       {
                 jumpingUp=false;	       
	       }
	   }
	   else
	   {
	     jumpY-=0.5;		   
	     if (jumpY<=0.00)
	      {
	        jumping=false;
                jumpingUp=true;	       


	      }
	   }    	  
      
      }     

	 return jumpY;
 
 }
void draw(void)
{


if(zoom==1){

	zTrans+=1.2;
}

else if(zoom==-1){

	zTrans-=1.2;
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
	
	if (!crowching) run+=0.01;	
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glPushMatrix();

	glTranslatef(0.0, -50.0, 0.0+zTrans);

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		
		if(transL>=40)
		{
			maxm=true;			
		} else if(transL <= 10)
		{
			maxm=false;
		}

		angle = 50*atan( (sin(run)*cos(run))/sqrt( (1-(sin(run)*sin(run)) ) ));
		if(maxm)
		{
			transL-=0.01;			
		} else {
			transL+=0.01;			
			angle = -angle;
		}

		glTranslatef(-50+(transL*3), 0.0, -150-(50*sqrt(1-(sin(transL)*sin(transL)))) +zTrans );
		glRotatef(angle, 0.0, 1.0, 0.0);	
		player.Draw(40, 46);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		xPlayer1+=xIncrement;
		if (xPlayer1 >=25.00) xPlayer1=24.99;
		glTranslatef(xPlayer1, jump(), -100+zTrans);
		
		if (jumping)
		 {
		     player2.Draw(67, 73);
		 }
               else if(crowching)
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
		glTranslatef(xPlayer1+50, 0.0, -100+zTrans);

               
                if(crowching)
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
			glTexCoord2f(0.0, 10.0-run); glVertex3f(-1000, -25.0, -1000);
			glTexCoord2f(10.0, 10.0-run); glVertex3f(-1000, -25.0, 1000);
			glTexCoord2f(10.0, 0.0-run); glVertex3f(1000, -25.0, 1000);			
			glTexCoord2f(0.0, 0.0-run); glVertex3f(1000, -25.0, -1000);
		glEnd();
		glDisable(GL_TEXTURE_2D);		
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);	
		glBindTexture(GL_TEXTURE_2D, pa);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0+run, 0.0); 
			glVertex3f(-1000, -25.0, -1000);
			glTexCoord2f(1.0+run, 0.0); 
			glVertex3f(1000, -25.0, -1000);
			glTexCoord2f(1.0+run, 1.0); 
			glVertex3f(1000, 1100.0, -1000);			
			glTexCoord2f(0.0+run, 1.0); 
			glVertex3f(-1000, 1100.0, -1000);
		glEnd();
		glDisable(GL_TEXTURE_2D);		
	glPopMatrix();	
	glPopMatrix();
	glFlush();
	SDL_GL_SwapBuffers();
	
}

int main(void)
{
	SDL_Surface *screen;
	int done;
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
	int audio_channels = 2;
	int audio_buffers = 4096;

       if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
                printf("Unable to initialize SDL: %s\n", SDL_GetError());
                return 1;
        }


	screen = SDL_SetVideoMode(800, 600, 0, SDL_OPENGL);
	
	if(!screen) 
	{
		fprintf(stderr, "Couldn't set 800x600 GL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) 
	{
		printf("Unable to open audio!\n");
		exit(1);
	}

	SDL_WM_SetCaption("Game Engine by Raydelto Hernandez And Argenis Bilbao", "Game Engine by Raydelto Hernandez And Argenis Bilbao");	
	init();
	done = 0;
  	music = Mix_LoadMUS("data/music/14.ogg");
	Mix_PlayMusic(music, -1); 
	Mix_FreeMusic(music);
	start = clock();

	while(!done) 
	{
		if(elapsed >= 0.0083)
		{
			start = clock();
			draw();
		        
		}
		end = clock();
		elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;	

		while(SDL_PollEvent(&event)) 
		{
			if(event.type == SDL_QUIT) 
			{
				done = 1;
			}
			
		      	if ( event.type == SDL_KEYDOWN ) 
			{

				if(event.key.keysym.sym == SDLK_UP)
                                   {
						jumping=true;

				   }
	

				if(event.key.keysym.sym == SDLK_DOWN)
                                   {
						crowching=true;

				   }


				
				if(event.key.keysym.sym == SDLK_RIGHT)
                                   {
                    			xIncrement=0.5;           
				   }


				if(event.key.keysym.sym == SDLK_LEFT)
                                   {
                    			xIncrement=-0.5;
           
				   }

				if(event.key.keysym.sym == SDLK_q)
                                   {
                    			done=1;
           
				   }
				if(event.key.keysym.sym == SDLK_h)
                                   {
                    			zoom=1;
           
				   }
				
				if(event.key.keysym.sym == SDLK_g)
                                   {
                    			zoom=-1;
           
				   }
				
				if(event.key.keysym.sym == SDLK_j)
                                   {
                    			zoom=0;
           
				   }			
				
			}


		      	if ( event.type == SDL_KEYUP ) 
			{

                             xIncrement=0.0;

				if(event.key.keysym.sym == SDLK_DOWN)
                                   {
						crowching=false;

				   }
				
			}

		}
	}
	
	SDL_Quit();	
	return 0;
}
