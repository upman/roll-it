#include "./headers/rectangle.h"
#include "./headers/configs.h"

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
	fixturedef.density = loadConfig("configs","rectangle","density");
	fixturedef.restitution = loadConfig("configs","rectangle","restitution");
	fixturedef.friction = loadConfig("configs","rectangle","friction");
	body->CreateFixture(&fixturedef);
}

void drawRect(b2Vec2* points, b2Vec2 center, float angle, GLuint texture)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glColor3ub(255,255,255);
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(points[0].x*M2P,points[0].y*M2P);

		glTexCoord2f(1, 0);
		glVertex2f(points[1].x*M2P,points[1].y*M2P);

		glTexCoord2f(1, 1);
		glVertex2f(points[2].x*M2P,points[2].y*M2P);

		glTexCoord2f(0, 1);
		glVertex2f(points[3].x*M2P,points[3].y*M2P);
		glEnd();
		glFlush();
	glPopMatrix();
}
