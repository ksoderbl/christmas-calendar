#include <stdio.h>
#include <string.h>
#include <time.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "main.h"
#include "stars.h"

static char *effect22_name = "GLUT Objects II";

/* state variables */

/*static int blend = 1;*/
static int frame_count = 0;      /* number of frames drawn */
#define NUMOBJECTS 6
static int object = 4;
#define NUMCOLORS 8
static int color  = 0;


static GLfloat rot[3] = {0.0, 0.0, 0.0};
/* initial rotation speeds of cubes */
static GLfloat w[3] = {0.5, 0.4, 0.3};
#define MY_Z0 5.0
static GLfloat my_z = MY_Z0;
/* star rotation */
static GLfloat srot[3] = {0.0, 0.0, 0.0};

static GLfloat global_ambient[]  = { 0.0, 0.0, 0.0, 1.0 };

static GLfloat light0_ambient[]  = { 0.1, 0.1, 0.0, 1.0 };
static GLfloat light0_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light0_position[] = { 1.0, 1.0, 15.0, 0.0 };

static GLfloat mat_ambient[]     = { 0.2, 0.2, 0.2, 1.0 };
static GLfloat mat_specular[]    = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_emission[]    = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat mat_shininess[]   = { 30.0 };

static GLfloat fontcolor[3] = {0.3, 0.8, 1.0};

static GLfloat color_array[NUMCOLORS][3] = 
{{ 0.0, 0.0, 0.0 },
 { 0.0, 0.0, 1.0 },
 { 0.0, 1.0, 0.0 },
 { 0.0, 1.0, 1.0 },
 { 1.0, 0.0, 0.0 },
 { 1.0, 0.0, 1.0 },
 { 1.0, 1.0, 0.0 },
 { 1.0, 1.0, 1.0 }};


static void effect22_reset(void)
{
	int i;
	
	for (i = 0; i < 3; i++) {
		rot[i] = 0.0;
		w[i] = 0.0;
	}
	my_z = MY_Z0;
}




static void effect22_display_cb(void)
{
	int i;
	char s[256];

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, my_z,        /* eye */
		  0.0, 0.0, my_z - 1.0,  /* ctr */
		  0.0, 1.0, 0.0);        /* up */

	glPushMatrix();
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


 	glColor3fv(color_array[color]);

	glPushMatrix();
	switch (object) {
	case 0:	glutSolidTetrahedron();	break;
	case 1:	glutSolidCube(1.0);	break;
	case 2: glutSolidOctahedron(); break;
	case 3: 
		glScalef(0.5, 0.5, 0.5);
		glutSolidDodecahedron();
		break;
	case 4:
		glutSolidIcosahedron();
		break;
	case 5: glutSolidTorus(0.4, 0.8, 30, 30); break;
	default:
		break;
	}


	glPopMatrix();
	glPopMatrix();


	glDisable(GL_LIGHTING);

	glRotatef(srot[0], 1.0, 0.0, 0.0);
	glRotatef(srot[1], 0.0, 1.0, 0.0);
	glRotatef(srot[2], 0.0, 0.0, 1.0);
	srot[0] += 0.1;
	srot[1] += 0.11;
	srot[2] += 0.12;


	stars_draw();

	if (messages_on() || pause_is_requested()) {
		sprintf(s, "w0: %.1f w1: %.1f w2: %.1f", w[0], w[1], w[2]);
		messages_print(s, frame_count, fontcolor);
	}


	glutPostRedisplay();
	glutSwapBuffers();

	/* activate pause if requested */
	if (pause_is_requested())
		pause_activate();

	/* switch color every 50 frames */
	if (!(frame_count % 50))
		color = (color + 1) % NUMCOLORS;

	frame_count++;
}

static void effect22_reshape_cb(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble)w/(GLdouble)h, 0.1, 1000.0);
}

static void effect22_keyboard_cb(unsigned char key, int x, int y)
{
	x=x;y=y;
	switch (key) {
	case ' ':
		object = (object + 1) % NUMOBJECTS;
		break;
	case 'm':
		messages_toggle();
		break;
	case 'p':
		pause_request();
		break;
	case 'r':
		effect22_reset();
		break;
	case 'x':
		w[2] += 0.1;
		break;
	case 'z':
		w[2] -= 0.1;
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
static void effect22_special_cb(int key, int x, int y)
{
	x=x;y=y;
	switch (key) {
	case GLUT_KEY_PAGE_UP:
		my_z -= 0.5;
		if (my_z <= 0.0)
			my_z = 0.0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		my_z += 0.5;
		break;
	case GLUT_KEY_LEFT:
		w[1] -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		w[1] += 0.1;
		break;
	case GLUT_KEY_UP:
		w[0] -= 0.1;
		break;
	case GLUT_KEY_DOWN:
		w[0] += 0.1;
		break;
	default:
		break;
	}
}




static int effect22_init_cb(struct effect *ep)
{
	(void)ep;

#if 0
	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
#endif
	glEnable(GL_NORMALIZE);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	/*glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);*/
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	/* global ambient light intensity */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	/* local viewer ? (GL_FALSE is default) */
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	
	/* one-/twosided light model ? (GL_FALSE is default) */
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	/* allow changing of material using glColor */
	/*glColorMaterial(GL_FRONT, GL_AMBIENT);*/
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	/*glColorMaterial(GL_FRONT, GL_SPECULAR);*/
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	object = 4;
	frame_count = 0;

	/*effect22_reset();*/
	return 0;
}



static void effect22_cleanup_cb(struct effect *ep)
{
	ep=ep;

	return;
}


int effect22_register(struct effect *ep)
{
	ep->e_init     = effect22_init_cb;
	ep->e_cleanup  = effect22_cleanup_cb;

	ep->e_display  = effect22_display_cb;
	ep->e_reshape  = effect22_reshape_cb;
	ep->e_keyboard = effect22_keyboard_cb;
	ep->e_special  = effect22_special_cb;
	/*ep->e_mouse    = effect22_mouse_cb;*/
	/*ep->e_motion        = effect22_motion_cb;*/
	/*ep->e_passivemotion = effect22_passivemotion_cb;*/
	/*ep->e_idle          = effect22_idle_cb;*/

	ep->e_name     = effect22_name;
	return 0;
}
