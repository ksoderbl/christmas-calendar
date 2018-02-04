#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "calendar.h"
#include "main.h"
#include "quit.h"

static char *quit_message = "Really Quit ? (y/n)";
static int quitting = 0;

/*
 * To use this remember to set orthogonal perspective
 */
static void print_string(void *font, char *s)
{
	int len, i;

	len = (int)strlen(s);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, s[i]);
	}
}




int quit_is_requested(void)
{
	return quitting;
}

void quit_message_print(void)
{
	GLint vp[4];
	GLdouble width=0.0, height=0.0;
	GLint msgx, msgy;
	
	glGetIntegerv(GL_VIEWPORT, vp);
	width = (GLdouble) vp[2];
	height = (GLdouble) vp[3];
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	msgx = (vp[2] / 2) - strlen (quit_message) * 6  ;
	msgy = (vp[3] / 2) - 12;
	
	/* irix opengl draws these in reverse order */
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2i(msgx+2, msgy+2);
	print_string(GLUT_BITMAP_TIMES_ROMAN_24, quit_message);

	glColor3f(0.0, 0.0, 0.0);
	glRasterPos2i(msgx, msgy);
	print_string(GLUT_BITMAP_TIMES_ROMAN_24, quit_message);
		
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


static void quit_keyboard_cb(unsigned char key, int x, int y)
{
	switch (key) {
	case 'Y':
	case 'y':
		exit(0);
	case 'N':
	case 'n':
	case ASCII_ESC:
		quitting = 0;
		calendar_restore_glut_callbacks();
		break;
	default:
		break;
	}
	glutPostRedisplay();
	x = x; y = y;
}



/* let's just ignore these while quitting */
static void quit_special_cb(int key, int x, int y)
{
	key = key; x = x; y = y;

	printf("quit_special_cb: key = %x, x = %d, y = %d\n", key, x, y);
}

static void quit_mouse_cb(int button, int state, int x, int y)
{
	button = button; state = state; x = x; y = y;

	printf("quit_mouse_cb: button = %d, state = %d, x = %d, y = %d\n",
		  button, state, x, y);
}


void quit_request(void)
{
	printf("%s\n", quit_message);
	quitting = 1;
	glutKeyboardFunc(quit_keyboard_cb);
	glutSpecialFunc(quit_special_cb);
	glutMouseFunc(quit_mouse_cb);
	glutPostRedisplay();
}
