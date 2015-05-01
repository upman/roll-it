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
void drawCircle(b2Vec2 center, float angle, float radius,GLuint texture){
	glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glColor3ub(255,255,255);
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
		glBegin(GL_POLYGON);
    float numSegments,radian,x,y;
    for (numSegments=0.0; numSegments<360.0; numSegments+=2.0)
    {
      radian = numSegments * (M_PI/180.0f);

      float xcos = (float)cos(radian);
      float ysin = (float)sin(radian);
      x= xcos*radius;
      y= ysin*radius;
      float tx = xcos*0.5 + 0.5;
      float ty = ysin*0.5 +0.5;
      glTexCoord2f(tx, ty);
      glVertex2f(x*M2P, y*M2P);
    }
    glEnd();
		glFlush();
	glPopMatrix();
}
