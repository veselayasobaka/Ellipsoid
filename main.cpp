#include <GL/glut.h>    
#include <GL/gl.h>	
#include <GL/glu.h>	
#include <unistd.h>
#include <math.h>     
#include <iostream>
using namespace std;

#define ESCAPE 27

int window; 

/* rotation angle */
float rtri = 0;
float t = 0;

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		
  glClearDepth(1.0);				
  glDepthFunc(GL_LESS);			        
  glEnable(GL_DEPTH_TEST);		        
  glShadeModel(GL_SMOOTH);			
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();	
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	
  glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized*/
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}
void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
{
	float aStep = (M_PI) / (float)uiSlices;
	float bStep = (M_PI) / (float)uiStacks;
	for(float alpha = -M_PI/2; alpha <= (M_PI/2)+.0001; alpha += aStep)
	{
		glColor3f(2.55, 1.79, 0);
		if(alpha>0)
		    glColor3f(0, 0, 0.6);
		glBegin(GL_TRIANGLE_STRIP);
		for(float beta = 0; beta <= 2*M_PI+.0001; beta += bStep)
		{
			glVertex3f(fA * cos(alpha) * cos(beta), fB * cos(alpha) * sin(beta), fC * sin(alpha));
			glVertex3f(fA * cos(alpha+aStep) * cos(beta), fB * cos(alpha+aStep) * sin(beta), fC * sin(alpha+aStep));
		}
		glEnd();
	}
}
float z = 2;
float v = 0;
float timer = 0;
float zCoord()
{
	t = 0.01;
	v+=0.01*t;
	z -= 10*v;
	cout << z << ' ' << v << endl;
	if (z<-1)
		{
		v*=-1;
		}
	return z;
}

/* The main drawing function. */
void DrawGLScene()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

  glLoadIdentity();				// Reset The View
  glTranslatef(0, 0 ,-6);
  glBegin(GL_POLYGON);				// drawing
  glColor3f(0.2f,0.5f,0.0f);			// Set The Color
  glVertex3f( 2.0f,-1.0f, 1.0f);		// Top Right 
  glVertex3f(-2.0f,-1.0f, 1.0f);		// Top Left 
  glVertex3f(-2.0f,-1.0f,-1.0f);		// Bottom Left 
  glVertex3f( 2.0f,-1.0f,-1.0f);		// Bottom RightOf 		
  glEnd();

  glLoadIdentity();
  glTranslatef(0, 0.3,-6);
  glRotatef(100, 0, 0, 0);
  glRotatef(rtri, 1, 0, 0);
  DrawEllipsoid(30, 30, 0.1, 0.4, 0.9);
  rtri-=2;					
  glutSwapBuffers();
  t += 0.001;
  glEnd();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* avoid thrashing this call */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) 
    { 
      /* shut down our window */
      glutDestroyWindow(window); 
      
      /* exit the program...normal termination. */
      exit(0);                   
    }
}

int main(int argc, char **argv) 
{  
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */  
  glutInit(&argc, argv);  

  /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffered for automatic clipping */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* get a 640 x 480 window */
  glutInitWindowSize(800, 600);  

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  window = glutCreateWindow("ellipsoid");  

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);  

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */
  InitGL(640, 480);
  
  /* Start Event Processing Engine */  
  glutMainLoop();  

  return 1;
}

