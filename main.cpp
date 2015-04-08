#include<stdio.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>
const int WIDTH=640;
const int HEIGHT=480;
const float M2P=20;
const float P2M=1/M2P;
const float PI=3.142;
b2World* world;

b2Body* addRect(int x,int y,int w,int h,bool dyn=true)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M,y*P2M);
	if(dyn)
		bodydef.type=b2_dynamicBody;
	else
	  bodydef.type=b2_staticBody;
		
	b2Body* body=world->CreateBody(&bodydef);
	
	b2PolygonShape shape;
	shape.SetAsBox(P2M*w/2,P2M*h/2);
	
	b2FixtureDef fixturedef;
	fixturedef.shape=&shape;
	fixturedef.density=0.1;
	body->CreateFixture(&fixturedef);
}

void drawSquare(b2Vec2* points,b2Vec2 center,float angle)
{
	glColor3f(1,1,1);
	glPushMatrix();
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
		glBegin(GL_QUADS);
			for(int i=0;i<4;i++)
				glVertex2f(points[i].x*M2P,points[i].y*M2P);
		glEnd();
		glFlush();
	glPopMatrix();
}

void init()
{
	glMatrixMode(GL_PROJECTION);
		glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	world=new b2World(b2Vec2(0.0,2.0));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30,false);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glLoadIdentity();
	b2Body* tmp=world->GetBodyList();
	b2Vec2 points[4];
	while(tmp)
	{
		for(int i=0;i<4;i++)
			points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
	
		drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
		tmp=tmp->GetNext();
	}
		glutSwapBuffers();
}

void mouse(int button, int state, int x, int y){
  if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    addRect(x,y,20,20,true);//add a square dynamic box
  } else
    if(button==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
    addRect(x,y,100,20,false);// Add a flat, static rectangle
  }
  
}

void step(){
  float m=glutGet(GLUT_ELAPSED_TIME)/(1.0/30.0);
  if(floor(m)==m){//If the time is a multiple of 1/30 th of a second, redisplay
    world->Step(1.0/30.0,8,3);
    glutPostRedisplay();
  }
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(640,480);
	glutCreateWindow("Boxes");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
  glutIdleFunc(step);
	glutMainLoop();
}
