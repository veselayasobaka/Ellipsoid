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
typedef Matrix<float, 3, 3> Matrix3;
#define ESCAPE 27
int window; 

struct RigidEllipsoid
{
    /* Constant quantities */
    Matrix3 Ibody;
    Matrix3 Ibodyinv;

    /* State variables */
    Vector3 x;
    Matrix3 R;
    Vector3 v;
    Vector3 L;

    /* Derived quantities (auxiliary variables) */
    Matrix3 Iinv;

    Vector3 omega;

    /* Computed quantities */
    Vector3 force;
    Vector3 torque;
    void printVertex(const Vector3 &v)
    {
        glVertex3f(v[0], v[1], v[2]);
    }

    /* Methods */
    RigidEllipsoid()
    {
    x[0] = x[2] = 0;
    x[1] = -4;
    v[0] = v[1] = v[2] = 0;
    L[0] = L[1] = L[2] = 1;
    Ibody << x[1]*x[1]+x[2]*x[2], 0, 0,
             0, x[0]*x[0]+x[2]*x[2], 0,
             0, 0, x[1]*x[1]+x[2]*x[2];
    Ibodyinv = Ibody.inverse();
    R << 1, 0, 0,
         0, 1, 0,
         0, 0, 1;
    }
    
    /**
     * \brief
     * \details
     * \param[in] uiStacks - 
     * \param[in] uiSlices - 
     * \param[in] fA - 
     * \param[in] fB - 
     * \param[in] fC - 
     */
    void DrawEllipsoid(unsigned int uiStacks, unsigned int uiSlices, float fA, float fB, float fC)
    {
        glLoadIdentity();
        glTranslatef(x[0], x[2], x[1]);
        glBegin(GL_LINES);
        float aStep = (M_PI) / (float)uiSlices;
        float bStep = (M_PI) / (float)uiStacks;

        for(float alpha = -M_PI/2; alpha <= (M_PI/2)+.0001; alpha += aStep)
        {
            glColor3f(0, 1, 0);
            if(alpha < 0 && alpha > -M_PI/4)
                glColor3f(1, 0, 1);
            if(alpha > M_PI/4)
                glColor3f(1, 0, 1);

            glBegin(GL_TRIANGLE_STRIP);
            for(float beta = 0; beta <= 2*M_PI+.0001; beta += bStep)
            {
                Vector3 v1(fA * cos(alpha) * cos(beta), fB * cos(alpha) * sin(beta), fC * sin(alpha));
                Vector3 v2(fA * cos(alpha+aStep) * cos(beta), fB * cos(alpha+aStep) * sin(beta), fC * sin(alpha+aStep));
                printVertex(R*v1);
                printVertex(R*v2);
            }
            glEnd();
        }
        glEnd();
    }
/*q.normalize();
  Rot = q.toMatrix3Matrix();*/
};

RigidEllipsoid Ellipsoid;

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
    if (Height==0)                // Prevent A Divide By Zero If The Window Is Too Small
        Height=1;

    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
    static float t = 0;        // time
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);    // Clear The Screen And The Depth Buffer

    glLoadIdentity();                // Reset The View
    glTranslatef(0.0f,0.0f,-4.0f);
    glBegin(GL_POLYGON);                // start drawing a pyramid
    glColor3f(0.2f,0.5f,0.0f);            // Set The Color To Orange
    glVertex3f( 3.0f,-1.0f, 1.0f);        // Top Right Of The Quad (Bottom)
    glVertex3f(-3.0f,-1.0f, 1.0f);        // Top Left Of The Quad (Bottom)
    glVertex3f(-3.0f,-1.0f,-1.0f);        // Bottom Left Of The Quad (Bottom)
    glVertex3f( 3.0f,-1.0f,-1.0f);        // Bottom Right Of The Quad (Bottom)
    glEnd();
    Ellipsoid.DrawEllipsoid(30, 30, 0.1, 0.2, 0.5);
    t += 0.05;

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
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
    glutInitWindowSize(800, 600);  
    glutInitWindowPosition(0, 0);  
    window = glutCreateWindow("Help me, I think I'm falling");  
    glutDisplayFunc(&DrawGLScene);  
    glutIdleFunc(&DrawGLScene);
    glutReshapeFunc(&ReSizeGLScene);
    glutKeyboardFunc(&keyPressed);
    InitGL(640, 480);
    glutMainLoop();  
    return 1;
}

