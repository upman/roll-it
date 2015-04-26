#include "./headers/triangle.h"
#include "./headers/configs.h"
b2Body* addTriangle(int cx,int cy, int scale, b2World* world, bool dyn=true)
{
	b2BodyDef bodydef;
	bodydef.position.Set(cx*P2M,cy*P2M);//Co-ordinates of the center of the rectangle
	if(dyn)
		bodydef.type=b2_dynamicBody;
	else
	  bodydef.type=b2_staticBody;

	b2Body* body=world->CreateBody(&bodydef);

	b2Vec2 vertices[3];
  vertices[0].Set(-3*scale*P2M, -2*scale*P2M);
	vertices[1].Set(3*scale*P2M,  -2*scale*P2M);//The vertices need to be specified in counter clockwise order. !!!!Important!!!!
	vertices[2].Set(0*scale*P2M,  4*scale*P2M);

	b2PolygonShape shape;
	shape.Set(vertices, 3);
	b2FixtureDef fixturedef;
	fixturedef.shape=&shape;
	fixturedef.density=loadConfig("configs","triangle","density");
	fixturedef.friction = loadConfig("configs","triangle","friction");
	body->CreateFixture(&fixturedef);
}

void drawTriangle(b2Vec2* points, b2Vec2 center, float angle){
	glColor3f(0,1,1);
	glPushMatrix();
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
		glBegin(GL_POLYGON);
			for(int i=0;i<3;i++)
				glVertex2f(points[i].x*M2P,points[i].y*M2P);
		glEnd();
		glFlush();
	glPopMatrix();
}
