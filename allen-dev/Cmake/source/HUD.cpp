//#ifdef __APPLE__
//#include <GLUT/glut.h>
//#endif
//#ifdef __unix__
//#include <GL/glut.h>
//#endif
//#include "../libraries/HUD.h"
////#include "../libraries/MatrixStack.h"
//
//int HUD::width = 1;
//int HUD::height = 1;
//
//void HUD::setWidthHeight(int w, int h)
//{
//	width = w;
//	height = h;
//}
//
//void HUD::drawString(float x, float y, const char *str)
//{
//	glColor3f(0.0f, 1.0f, 0.0f);
//	MatrixStack P;
//	P.ortho(0, width, 0, height, -1, 1);
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
////	glLoadMatrixf(P.topMatrix().data());
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glRasterPos3f(x, y, 1.0);
//	for(int i = 0; i < strlen(str); ++i) {
//		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
//	}
//	glPopMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//}
