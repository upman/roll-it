#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include <string.h>
#include "./components/headers/circle.h"
#include "./components/headers/rectangle.h"
#include "./components/headers/triangle.h"
b2World* world;

void init()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	world=new b2World(b2Vec2(0.0,2.0));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30, world, false,false);
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
		if(tmp->GetFixtureList()->GetType() ==  b2Shape::e_circle){
	    drawCircle(tmp->GetWorldCenter(), tmp->GetAngle(),((b2CircleShape*)(tmp->GetFixtureList()->GetShape()))->m_radius);
	    tmp=tmp->GetNext();
	  }
		else if(((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertexCount() == 3){
			for(int i = 0; i < 3; i++)
				points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);

			drawTriangle(points, tmp->GetWorldCenter(), tmp->GetAngle());
			tmp=tmp->GetNext();
		}
		else
		{
			if(tmp->GetUserData()){
					if(strcmp((char*)tmp->GetUserData(),"rotate")){
						tmp->SetTransform(tmp->GetPosition(),(tmp->GetAngle()+0.01f));
						//increase angle by 0.009 radians at every step
					}
				}
  		  for(int i = 0; i < 4; i++)
	  		  points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);

	  	    drawRect(points,tmp->GetWorldCenter(),tmp->GetAngle());
	  	    tmp=tmp->GetNext();
	  }
	}
		glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
	if(key == ' '){
		addRect(x, y, 100, 20, world, false,true);
	}
	else if(key == 's'){
		addRect(x, y, 20, 20, world, true,false);
	}
	else if(key == 'a'){
		addCircle(x, y, 20, world, true);
	}
	else if(key == 'd'){
		addTriangle(x, y, 4, world, true);
	}
	else if(key == 'S'){
		addRect(x, y, 20, 20, world, false,false);
	}
	else if(key == 'A'){
		addCircle(x, y, 30, world, false);
	}
	else if(key == 'D'){
		addTriangle(x, y, 4, world, false);
	}
}

void motion(int x, int y){
  addRect(x,y,1,1, world, false, false);
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
	glutCreateWindow("Roll It");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
  glutIdleFunc(step);
	glutMainLoop();
}
