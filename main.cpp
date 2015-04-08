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

void addCircle(int x, int y, int r, bool dyn=true){

  b2BodyDef bodydef;
	bodydef.position.Set(x*P2M,y*P2M);
	if(dyn)
		bodydef.type=b2_dynamicBody;
	else
	  bodydef.type=b2_staticBody;
	b2Body* body=world->CreateBody(&bodydef);

  b2CircleShape circleShape;
  circleShape.m_p.Set(0, 0); //position, relative to body position
  circleShape.m_radius = 1; //radius

  b2FixtureDef fixturedef;
  fixturedef.shape = &circleShape; //this is a pointer to the shape above
  fixturedef.friction = 0.00f;
  fixturedef.restitution = 0.5f;
  body->CreateFixture(&fixturedef); //add a fixture to the body
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

void drawCircle(b2Vec2 center, float angle){
	glPushMatrix();
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
      glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glVertex2f(0.0f, 0.0f);       // Center of circle
        int numSegments = 100;
        for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
          angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
          glVertex2f(cos(angle) * 20, sin(angle) * 20);
        }
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
		if(tmp->GetFixtureList()->GetType() ==  b2Shape::e_circle){
	    drawCircle(tmp->GetWorldCenter(), tmp->GetAngle());
	    tmp=tmp->GetNext();
	  }
		else{
  		  for(int i=0;i<4;i++)
	  		  points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);

	  	    drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
	  	    tmp=tmp->GetNext();
	  	  }
	}
		glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y){
  if(key == ' '){
    addRect(x,y,100,20,false);
  }else if(key == 's'){
    addRect(x,y,20,20,true);
  }else if(key == 'a'){
    addCircle(x,y,20,true);
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
	glutKeyboardFunc(keyboard);
  glutIdleFunc(step);
	glutMainLoop();
}
