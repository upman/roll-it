#include "./headers/circle.h"
#include "./headers/configs.h"
GLfloat vertices[360][3];
GLfloat textureCoords[360][3];

b2Body* addCircle(int x, int y, int r,  b2World* world, bool dyn=true){
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
  fixturedef.density = loadConfig("configs","circle","friction");
  body->CreateFixture(&fixturedef); //add a fixture to the body
  return body;
}
void drawCircle(b2Vec2 center, float angle, float radius,GLuint texture){
	glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glColor3ub(255,255,255);
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
    glScalef(radius*M2P,radius*M2P,0);
      glDrawArrays(GL_POLYGON, 0, 360);
		glFlush();
	glPopMatrix();
}

void computeCircleVertices(){
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  float numSegments,radian,x,y;
  for (numSegments=0.0; numSegments<360.0; numSegments+=1.0)
  {
    radian = numSegments * (M_PI/180.0f);

    float xcos = (float)cos(radian);
    float ysin = (float)sin(radian);
    x= xcos;
    y= ysin;
    float tx = xcos*0.5 + 0.5;
    float ty = ysin*0.5 +0.5;
    textureCoords[(int)numSegments][0] = tx;
    textureCoords[(int)numSegments][1] = ty;
    textureCoords[(int)numSegments][2] = 0;

    vertices[(int)numSegments][0] = x;
    vertices[(int)numSegments][1] = y;
    vertices[(int)numSegments][2] = 0;
  }
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(3,GL_FLOAT,0,textureCoords);
}
