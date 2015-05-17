#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include <string.h>
#include <FTGL/ftgl.h>
#include "./components/headers/circle.h"
#include "./components/headers/rectangle.h"
#include "./components/headers/triangle.h"
#include "./components/headers/configs.h"
#include "./components/headers/textures.h"
b2World* world;
b2Body* selectedBody;
FTGLPixmapFont* font;
int introScreenFlag=1;
extern GLuint RectTexture;
extern GLuint TriTexture;
extern GLuint BackTexture;
extern GLuint CircTexture;

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

void renderWorld(){
	b2Body* tmp=world->GetBodyList();
	b2Vec2 points[4];
	while(tmp)
	{
		if(tmp->GetFixtureList()->GetType() ==  b2Shape::e_circle){
	    drawCircle(tmp->GetWorldCenter(), tmp->GetAngle(),((b2CircleShape*)(tmp->GetFixtureList()->GetShape()))->m_radius,CircTexture);
	    tmp=tmp->GetNext();
	  }
		else if(((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertexCount() == 3){
			for(int i = 0; i < 3; i++)
				points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);

			drawTriangle(points, tmp->GetWorldCenter(), tmp->GetAngle(),TriTexture);
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

	  	    drawRect(points,tmp->GetWorldCenter(),tmp->GetAngle(),RectTexture);
	  	    tmp=tmp->GetNext();
	  }
	}
}

void renderBackground(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, BackTexture);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glColor3ub(255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0, 0.0f);	// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex2f(640.0f, 0.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex2f(640.0f, 480.0f);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f,  480.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glLoadIdentity();
	renderBackground();
	if(introScreenFlag){
		glPushMatrix();
		glTranslatef(100,0,0);
		font->FaceSize(50);
		font->Render("Awesome intro screen",-1,FTPoint(0, 300, 0));
		glPopMatrix();
	}
	else{
		renderWorld();
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
	else if(key == 'p'){
		screenshot();
	}
}

class WorldQueryCallback : public b2QueryCallback {
  public:
      bool ReportFixture(b2Fixture* fixture) {
					selectedBody=fixture->GetBody();
          return false;//Stop after finding the first fixture
      }
};

void mouse(int button, int state, int x, int y){
	if(glutGetModifiers()==GLUT_ACTIVE_CTRL){
		return;
	}
	if (button == GLUT_LEFT_BUTTON){
		if(selectedBody && state == GLUT_UP){
			selectedBody->SetAwake(true);
			selectedBody = NULL;
		}

		if (state == GLUT_DOWN){
			b2Vec2 p;
			b2Vec2 d;
			p.Set((float)x*P2M,(float)y*P2M);
			d.Set(0.001f, 0.001f);

			b2AABB aabb;
			aabb.lowerBound = p - d;
			aabb.upperBound = p + d;

			// Query the world for overlapping fixtures on aabb.
			WorldQueryCallback  callback;
			world->QueryAABB(&callback, aabb);
		}
	}
}

void motion(int x, int y){
	if(selectedBody){
		selectedBody->SetTransform(b2Vec2(x*P2M,y*P2M),selectedBody->GetAngle());
	}
	else{
		addRect(x,y,1,1, world, false, false);
	}
}

void step(){
  float m=glutGet(GLUT_ELAPSED_TIME)/(1.0/30.0);
  if(floor(m)==m){//If the time is a multiple of 1/30 th of a second, redisplay
    world->Step(1.0/30.0,8,3);
    glutPostRedisplay();
  }
}

void init()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	LoadAllTextures();
}

void initSimulation(){
	float gravity = loadConfig("configs","world","gravity");
	world=new b2World(b2Vec2(0.0,gravity));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30, world, false,false);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(step);
}

void initIntro(){
		font = new FTGLPixmapFont("../fonts/chawp.ttf");
		if(font->Error()){
			printf("\nError loading font!\n");
			exit(0);
		}
}

void introKeyboard(unsigned char key, int x, int y){
		if(key==' '){
			initSimulation();
			introScreenFlag=0;
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
	font = new FTGLPixmapFont("../fonts/chawp.ttf");
	glutInitWindowSize(window_width,window_height);
	glutCreateWindow("Roll It");
	init();
	initIntro();
	createMenu();
	glutDisplayFunc(display);
	glutKeyboardFunc(introKeyboard);
	//TODO:Fix
	display();//Doesn't seem to work without this explicit call to display
	glutMainLoop();
}
