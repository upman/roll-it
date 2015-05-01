#include "./headers/circle.h"
#include "./headers/configs.h"
void addCircle(int x, int y, int r,  b2World* world, bool dyn=true){
  b2BodyDef bodydef;
	bodydef.position.Set(x*P2M,y*P2M);
	if(dyn)
		bodydef.type=b2_dynamicBody;
	else
	  bodydef.type=b2_staticBody;
	b2Body* body=world->CreateBody(&bodydef);

  b2CircleShape circleShape;
  circleShape.m_p.Set(0, 0); //position, relative to body position
  circleShape.m_radius = r*P2M; //radius

  b2FixtureDef fixturedef;
  fixturedef.shape = &circleShape; //this is a pointer to the shape above
  fixturedef.friction = loadConfig("configs","circle","friction");
  fixturedef.restitution = loadConfig("configs","circle","restitution");
  body->CreateFixture(&fixturedef); //add a fixture to the body
}
void drawCircle(b2Vec2 center, float angle, float radius){
	glPushMatrix();
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
		glColor3f(0.0f, 0.0f, 1.0f);  // Blue
		glBegin(GL_POLYGON);
    glColor3f(0.0f, 0.0f, 1.0f);  // Blue
    glVertex2f(0.0f, 0.0f);       // Center of circle
    int numSegments = 100;
    for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
        angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
        glVertex2f(cos(angle) * radius*M2P, sin(angle) * radius*M2P);
    }
    glEnd();
		glFlush();
	glPopMatrix();
}
