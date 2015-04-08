#include "./headers/rectangle.h"

b2Body* addRect(int x,int y,int w,int h, b2World* world, bool dyn=true)
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

void drawRect(b2Vec2* points,b2Vec2 center,float angle)
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
