#include <iostream>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/CMath.h"
#include "Math/Quaternion.h"
#include "StackAllocator.h"
#include "Math/EulerAngles.h"
#include "Physic/Particle.h"
#include "Physic/pfgen.h"

#include "gl\glut.h"
#include <Windows.h>
float lookAngle = 0.0;
float lookAngle2 = 0.0f;
using namespace std;
Matrix4 Look(false);
Matrix4 Per(false);
void initialise()
{
	//Ani.PrintfFrame(0,NULL);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 	
	glClearDepth(1.0f);  	
	
	glEnable(GL_DEPTH_TEST);  
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(Per.ToFloatPtr());
	//gluPerspective(30.,4.0/3.0,5.,1000.);
}

void DrawOriginLines()
{
	float size;
	size = 25.0f;
	glDisable(GL_LIGHTING);
	//glDisable(GL_CULL_FACE);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_LINES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-size, 0.0f, 0.0f);
		glVertex3f( size, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -size, 0.0f);
		glVertex3f(0.0f,  size, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -size);
		glVertex3f(0.0f, 0.0f, size);
	
	glEnd();

	glPointSize(8.0f);
	glBegin(GL_POINTS);
		glVertex3f(size, 0.0f, 0.0f);
		glVertex3f(0.0f, size, 0.0f);
		glVertex3f(0.0f, 0.0f, size);
	glEnd();
	glPointSize(1.0f);
	glEnable(GL_LIGHTING);
	//glEnable(GL_CULL_FACE);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void display()  
{
	glutPostRedisplay();
	static float prev;
	static float time = 0;
	float curr = GetTickCount( ) / 1000.0;
	if (curr - prev < 0.001f) return;
	static bool tas = false;

	time += curr - prev;
	prev = curr;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glClearColor(0.4,0.5,0.6,0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  	
	glMultMatrixf(Look.ToFloatPtr());
	//gluLookAt(t.x,t.y,t.z,0,0,0,0,1,0);
	
	float t = Math::FAbs(time - 3.0f)/ 3.0f;
	//cout<< t <<endl;
	if(time>3.0f) time = 0.0f;
	glPushMatrix();
	glColor3f(0.1, 1.0f, 0.5f);
	Matrix4 World;
	//World.Translate(10,10,0);
	Quaternion q1,q2, q;
	q.setRotateYAxis(45);
	World = q.ToMatrix();


	//World.Translate(10, 10,0);
	mat4 W1;W1.Identity();W1.Translate(10,0,0);
	mat4 W2;W2.Identity();W2.Translate(0,20,20);
	//World = W2*World;
	//glMultMatrixf(World.ToFloatPtr());
	//glTranslatef(10,10,10);
	glColor3f(0.0f, 1.0f, 1.0f);

	glutSolidTeapot(5);

	
	glPopMatrix();
	
	DrawOriginLines();
	
	glutSwapBuffers();
}

void specialKeys(int key, int x, int y)
{
	if(key == GLUT_KEY_LEFT) lookAngle += 5.0;
	if(key == GLUT_KEY_RIGHT) lookAngle -= 5.0;
	if(key == GLUT_KEY_UP) lookAngle2 += 5.0;
	if(key == GLUT_KEY_DOWN) lookAngle2 -= 5.0;
	if(key == GLUT_KEY_F2) 
	{
		lookAngle = lookAngle2 = 0.0f;
	}
	//cout<<lookAngle<<endl;
	Look = Math::LookAt(Vec3(100.0f*Math::Cos(Math::ToRadian(lookAngle)), 100.0f, 100.0f*Math::Sin(Math::ToRadian(lookAngle))), Vec3(0.f, 0.f, 0.f),Vec3(0., 1.f, 0.f));
	
	//Look = Math::LookAt(Vec3(0, 50.0f, 0.0f), Vec3(0.f, 0.f, 0.f),Vec3(0., 1.f, 0.f));
	//t = Vec3(25.0f*Math::Cos(Math::ToRadian(lookAngle)), 10.0, 25.0f*Math::Sin(Math::ToRadian(lookAngle)));
	
	glutPostRedisplay();
}


using namespace std;
void mouse( int x, int y)
{
	static int lastx = 0, lasty = 0;
	int dx = x - lastx;
	int dy = y - lasty;
	lastx = x;
	lasty = y;
	cout<< "<" << dx << "," << dy << ">\n";

}
static int window;
static int menu_id;
static int submenu_id;
static int value = 0; 

void menu(int num){
  if(num == 0){
    glutDestroyWindow(window);
    exit(0);
  }else{
    value = num;
  }
  glutPostRedisplay();
} 
void createMenu(void){     
	submenu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Sphere", 2);
    glutAddMenuEntry("Cone", 3);
    glutAddMenuEntry("Torus", 4);
    glutAddMenuEntry("Teapot", 5);     

	menu_id = glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 1);
    glutAddSubMenu("Draw", submenu_id);
    glutAddMenuEntry("Quit", 0);     
	glutAttachMenu(GLUT_RIGHT_BUTTON);
} 

int main_1(int argc, char** argv)
{
	Look = Math::LookAt(Vec3(25.0f*Math::Cos(Math::ToRadian(lookAngle)), 15.0f, 25.0f*Math::Sin(Math::ToRadian(lookAngle))), Vec3(0.f, 0.f, 0.f),Vec3(0., 1.f, 0.f));
	//gluPerspective(30.,4.0/3.0,5.,1000.);
	Per = Math::Perspective(30.0f, 4.0f/3.0, 1.0f, 1000.0f);
	//cout<<Look<<endl;


	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	
	glutInitWindowSize (800, 600); 
	glutInitWindowPosition (300, 100);
	window = glutCreateWindow ("Test");
	initialise ();
	createMenu(); 
	glutSpecialFunc(specialKeys);
	glutDisplayFunc(display); 
	glutSetCursor(GLUT_CURSOR_INHERIT);
	
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	return 0;
}

