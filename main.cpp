#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include <string.h>
#include "./components/headers/circle.h"
#include "./components/headers/rectangle.h"
#include "./components/headers/triangle.h"
#include "./components/headers/configs.h"
b2World* world;

void init()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	float gravity = loadConfig("configs","world","gravity");
	world=new b2World(b2Vec2(0.0,gravity));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30, world, false,false);
}

void resetWorld(){
	delete world;
	float gravity = loadConfig("configs","world","gravity");
  	world = new b2World(b2Vec2(0.0,gravity));
  	addRect(WIDTH/2,HEIGHT-50,WIDTH,30, world, false,false);
}
void menu(int value){
	if(value == 1){
		resetWorld();
	}
	else if(value == 0){
		exit(0);
	}
	else if(value == 2){
		readFiles();
	}
}
void createMenu(){
	glutCreateMenu(menu);
	glutAddMenuEntry("Reset",1);
	glutAddMenuEntry("Reload Configs",2);
	glutAddMenuEntry("Exit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
		float width = loadConfig("configs","rectangle","width");
		float height = loadConfig("configs","rectangle","height");
		addRect(x, y, width, height, world, false,true);
	}
	else if(key == 's'){
		float edge = loadConfig("configs","square","edge");
		addRect(x, y, edge, edge, world, true,false);
	}
	else if(key == 'a'){
		float radius = loadConfig("configs","circle","radius");
		addCircle(x, y, radius, world, true);
	}
	else if(key == 'd'){
		float scale = loadConfig("configs","triangle","scale");
		addTriangle(x, y, scale, world, true);
	}
	else if(key == 'S'){
		float edge = loadConfig("configs","square","edge");
		addRect(x, y, edge, edge, world, false,false);
	}
	else if(key == 'A'){
		float radius = loadConfig("configs","circle","static-radius");
		addCircle(x, y, radius, world, false);
	}
	else if(key == 'D'){
		float scale = loadConfig("configs","triangle","static-scale");
		addTriangle(x, y, scale, world, false);
	}else if(key == 'R'){
	  resetWorld();
	}
	else if(key == 'Q'){
		exit(0);
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
	readFiles();
	float window_height = loadConfig("configs","window","height");
	float window_width = loadConfig("configs","window","width");
	glutInitWindowSize(window_width,window_height);
	glutCreateWindow("Roll It");
	init();
	createMenu();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
  	glutIdleFunc(step);
	glutMainLoop();
}
