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
void draw_pixel(GLint cx, GLint cy)
{
    glVertex2i(cx,cy);
}
void plotpixels(GLint h, GLint k, GLint x, GLint y)
{
    draw_pixel(x+h,y+k);
    draw_pixel(-x+h,y+k);
    draw_pixel(x+h,-y+k);
    draw_pixel(-x+h,-y+k);
    draw_pixel(y+h,x+k);
    draw_pixel(-y+h,x+k);
    draw_pixel(y+h,-x+k);
    draw_pixel(-y+h,-x+k);
}
void Circle_draw(GLint h, GLint k, GLint r)  // Midpoint Circle Drawing Algorithm
{
    GLint d =  1-r, x=0, y=r;
    glBegin(GL_POLYGON);
    while(y > x)
    {
        plotpixels(h,k,x,y);
        if(d < 0)
            d+=2*x+3;
        else
        {
            d+=2*(x-y)+5;
            --y;
        }
        ++x;
    }
    plotpixels(h,k,x,y);
    glEnd();
}
void drawCircle(b2Vec2 center, float angle, float radius){
	glPushMatrix();
		glTranslatef(center.x*M2P,center.y*M2P,0);
		glRotatef(angle*180.0/M_PI,0,0,1);
		glColor3f(0.0f, 0.0f, 1.0f);  // Blue
		Circle_draw(0.0,0.0,radius*M2P);
		glFlush();
	glPopMatrix();
}
