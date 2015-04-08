#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include "../../config/constants.h"

b2Body* addRect(int,int,int,int, b2World*, bool);
void drawRect(b2Vec2*, b2Vec2,float);

#endif
