#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include "../../config/constants.h"

b2Body* addCircle(int, int, int, b2World*, bool);
void drawCircle(b2Vec2, float, float, GLuint);
void computeCircleVertices();

#endif
