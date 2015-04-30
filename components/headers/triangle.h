#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include "../../config/constants.h"

b2Body* addTriangle(int, int, int, b2World*, bool);
void drawTriangle(b2Vec2*, b2Vec2, float, GLuint);

#endif
