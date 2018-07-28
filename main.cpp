#include <iostream>
#include <GL/glut.h>    
#include <GL/gl.h>	
#include <GL/glu.h>	
#include <unistd.h> 
#include <math.h>    
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
using namespace std;
using namespace Eigen;
typedef Matrix<float, 3, 1> Vector3;
typedef Matrix<float, 3, 3> Rotation;
#define ESCAPE 27
int window; 

/* rotation angle for the triangle. */
float t = 0;
Rotation Rot;
void printVertex(const Vector3 &v)
{
    glVertex3f(v[0], v[1], v[2]);
}

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
void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC, Rotation Rot)
{
	float aStep = (M_PI) / (float)uiSlices;
	float bStep = (M_PI) / (float)uiStacks;
	for(float alpha = -M_PI/2; alpha <= (M_PI/2)+.0001; alpha += aStep)
	{

		glBegin(GL_TRIANGLE_STRIP);
		for(float beta = 0; beta <= 2*M_PI+.0001; beta += bStep)
		{
			Vector3 v1(fA * cos(alpha) * cos(beta), fB * cos(alpha) * sin(beta), fC * sin(alpha));
			Vector3 v2(fA * cos(alpha+aStep) * cos(beta), fB * cos(alpha+aStep) * sin(beta), fC * sin(alpha+aStep));
			printVertex(Rot*v1);
			printVertex(Rot*v2);
		}
		glEnd();
	}
}
/* The main drawing function. */
void DrawGLScene()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

  glLoadIdentity();				// Reset The View
  glTranslatef(0.0f,0.0f,-4.0f);
  glBegin(GL_POLYGON);				// start drawing a pyramid
  glColor3f(0.2f,0.5f,0.0f);			// Set The Color To Orange
  glVertex3f( 3.0f,-1.0f, 1.0f);		// Top Right Of The Quad (Bottom)
  glVertex3f(-3.0f,-1.0f, 1.0f);		// Top Left Of The Quad (Bottom)
  glVertex3f(-3.0f,-1.0f,-1.0f);		// Bottom Left Of The Quad (Bottom)
  glVertex3f( 3.0f,-1.0f,-1.0f);		// Bottom Right Of The Quad (Bottom)
  glEnd();

  glLoadIdentity();
  glTranslatef(0, 0, -4);
  glBegin(GL_LINES);
  Quaternionf q(cos(t), sin(t), sin(t), sin(t));
  q.normalize();
  Rot = q.toRotationMatrix();
  glColor3f(0, 0, 0);
  DrawEllipsoid(30, 30, 0.1, 0.2, 0.5, Rot);
  glEnd();
  t += 0.1;

  					
  glutSwapBuffers();
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
  window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");  

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

