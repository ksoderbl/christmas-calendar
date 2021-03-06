#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "main.h"
#include "messages.h"
#include "stars.h"

static int frame_count = 0; /* frames drawn */

static char *effect19_name = "Stars only";

/*static GLdouble width=0.0, height=0.0;*/
static GLfloat rot[3] = {0.0, 0.0, 0.0};
/* initial rotation speeds */
static GLfloat w[3] = {0.1, 0.1, 0.1};
#define MY_Z0 8.0
static GLfloat my_z = MY_Z0;

static GLfloat fontcolor[3] = {1.0, 1.0, 0.0};

static void effect19_display_cb(void)
{
	int i;
	char s[256];

	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, my_z,        /* eye */
		  0.0, 0.0, my_z - 1.0,  /* ctr */
		  0.0, 1.0, 0.0);        /* up */
	glRotatef(rot[0], 1.0, 0.0, 0.0);
	glRotatef(rot[1], 0.0, 1.0, 0.0);
	glRotatef(rot[2], 0.0, 0.0, 1.0);
	for (i = 0; i < 3; i++) {
		rot[i] += w[i];
		if (rot[i] >= 360.0)
			rot[i] -= 360.0;
		if (rot[i] < 0.0)
			rot[i] += 360.0;
	}

	stars_draw();

	glRotatef(90.0, 1.0, 0.0, 0.0);

	glRotatef(90.0, 0.0, 1.0, 0.0);

	glDisable(GL_LIGHTING);

	/* handle text to user and pause */
	if (messages_on() || pause_is_requested()) {
		sprintf(s, "w0: %.1f w1: %.1f w2: %.1f", w[0], w[1], w[2]);
		messages_print(s, frame_count, fontcolor);
	}

	glutPostRedisplay();
	glutSwapBuffers();

	/* activate pause if requested */
	if (pause_is_requested())
		pause_activate();

	frame_count++;
}

static void effect19_reshape_cb(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble)w/(GLdouble)h, 0.1, 1000.0);
}

static void effect19_keyboard_cb(unsigned char key, int x, int y)
{
	x=x;y=y;
	switch (key) {
	case 's': /* status */
	case 'm':
		messages_toggle();
		break;
	case 'p':
		pause_request();
		break;

	case ASCII_ESC:
		return_to_calendar();
		break;
	default:
		break;
	}

}

/* key is one of GLUT_KEY_F1-F12,
 * GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, 
 * GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN,
 * GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
 */
void effect19_special_cb(int key, int x, int y)
{
}

int effect19_init_cb(struct effect *ep)
{
	ep=ep;

	glBlendFunc(GL_ONE, GL_ONE);
	frame_count = 0; /* zero frame count */

	return 0;
}

void effect19_cleanup_cb(struct effect *ep)
{
	(void)ep;

	return;
}

int effect19_register(struct effect *ep)
{
	ep->e_init     = effect19_init_cb;
	ep->e_cleanup  = effect19_cleanup_cb;

	ep->e_display  = effect19_display_cb;
	ep->e_reshape  = effect19_reshape_cb;
	ep->e_keyboard = effect19_keyboard_cb;
	ep->e_special  = effect19_special_cb;
    /*ep->e_mouse    = effect19_mouse_cb;*/

	ep->e_name     = effect19_name;
	return 0;
}



