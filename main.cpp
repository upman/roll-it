#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include <string.h>
#include <FTGL/ftgl.h>
#include "./components/headers/circle.h"
#include "./components/headers/rectangle.h"
#include "./components/headers/triangle.h"
#include "./components/headers/configs.h"
#include "./components/headers/textures.h"
#include "./components/headers/main.h"
b2World* world;
b2Body* selectedBody;
FTGLPixmapFont* font;
int introScreenFlag=1;
int WIDTH,HEIGHT;
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
	while(tmp){
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
		else{
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
	glTexCoord2f(1.0f, 0.0f); glVertex2f(WIDTH, 0.0f);	// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex2f(WIDTH, HEIGHT);	// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f,  HEIGHT);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
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

void windowToWorld(int* x,int* y){
	GLint viewport[4]; //var to hold the viewport info
	GLdouble modelview[16]; //var to hold the modelview info
	GLdouble projection[16]; //var to hold the projection matrix info
	GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
	GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
	glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
	glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

	winX = (float)*x;
	winY = (float)viewport[3] - (float)*y;
	winZ = 0;

	//get the world coordinates from the screen coordinates
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

	*x = worldX;
	*y = worldY;
}

void keyboard(unsigned char key, int x, int y){
	windowToWorld(&x,&y);
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
	}
	else if(key == 'R'){
		resetWorld();
	}
	else if(key == 'Q'){
		exit(0);
	}
	else if(key == 'p'){
		screenshot();
	}
	else if(key == 'i'){
		switchToIntro();
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
	windowToWorld(&x,&y);
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
	windowToWorld(&x,&y);
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

void init(){
	glMatrixMode(GL_PROJECTION);
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0,0,0,1);
	LoadAllTextures();
	computeCircleVertices();
	font = new FTGLPixmapFont("../fonts/chawp.ttf");
	if(font->Error()){
		printf("\nError loading font!\n");
		exit(0);
	}
	float gravity = loadConfig("configs","world","gravity");
	world=new b2World(b2Vec2(0.0,gravity));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30, world, false,false);
}

void switchToSimulation(){
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(step);
	introScreenFlag = 0;
}

void switchToIntro(){
	glutKeyboardFunc(introKeyboard);
	glutMouseFunc(NULL);
	glutMotionFunc(NULL);
	glutIdleFunc(NULL);
	introScreenFlag = 1;
}

void introKeyboard(unsigned char key, int x, int y){
	if(key==' '){
		switchToSimulation();
	}
}

int main(int argc,char** argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
	readFiles();
	WIDTH = (int)loadConfig("configs","window","width");
	HEIGHT = (int)loadConfig("configs","window","height");
	font = new FTGLPixmapFont("../fonts/chawp.ttf");
	glutInitWindowSize(WIDTH,HEIGHT);
	glutCreateWindow("Roll It");
	init();
	switchToIntro();
	createMenu();
	glutDisplayFunc(display);
	glutKeyboardFunc(introKeyboard);
	//TODO:Fix
	display();//Doesn't seem to work without this explicit call to display
	glutMainLoop();
}
