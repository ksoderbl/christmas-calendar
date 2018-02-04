#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <math.h>
#include <time.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "messages.h"
#include "stars.h"


/* ---------------------------------------------------------------------- */


static void main_reshape_cb(int w, int h);
static void main_display_cb(void);
static void main_idle_cb(void);
static void darkness(void);
static void flash(void);

int dummy=0; /* don't change, see main.h */

/* ---------------------------------------------------------------------- */

static struct effect *effects = NULL;
struct effect *current_effect = NULL;

int init_effects(void)
{
	int err = 0, ei;
	
	effects = (effect_t *) calloc(NUMEFFECTS, sizeof(struct effect));
	if (!effects) {
		fprintf(stderr, "calloc failed\n");
		exit(1);
	}

	effects[0].e_register  = effect0_register;
	effects[1].e_register  = effect1_register;
	effects[2].e_register  = effect2_register;
	effects[3].e_register  = effect3_register;
	effects[4].e_register  = effect4_register;
	effects[5].e_register  = effect5_register;
	effects[6].e_register  = effect6_register;
	effects[7].e_register  = effect7_register;
	effects[8].e_register  = effect8_register;
	effects[9].e_register  = effect9_register;
	effects[10].e_register = effect10_register;
	effects[11].e_register = effect11_register;
	effects[12].e_register = effect12_register;
	effects[13].e_register = effect13_register;
	effects[14].e_register = effect14_register;
	effects[15].e_register = effect15_register;
	effects[16].e_register = effect16_register;
	effects[17].e_register = effect17_register;
	effects[18].e_register = effect18_register;
	effects[19].e_register = effect19_register;
	effects[20].e_register = effect20_register;
	effects[21].e_register = effect21_register;
	effects[22].e_register = effect22_register;
	effects[23].e_register = effect23_register;
	effects[24].e_register = effect24_register;

	/*
	 * call all effect registration functions to get function pointers
	 * to their init, main and cleanup functions
	 */
	for (ei = 0; ei < NUMEFFECTS; ei++) {
		err = (*effects[ei].e_register)(&effects[ei]);
		if (err < 0) {
			fprintf(stderr, "registration of effect %d is a stub\n",ei);
		} else {
			printf("effect%d name         : %s\n", ei, effects[ei].e_name);
		}
	}

	stars_make();

	return 0;
}

/* ---------------------------------------------------------------------- */



static int next_effect = -1;

/*
 * Function main_run_effect should be called from an affect to
 * relinquish control. If used from the calendar it will direct
 * the main code to start some other effect.
 * The other effects should use main_run_effect(0) or
 * return_to_calendar().
 */
void main_run_effect(int ei)
{
	int err, old;
	GLint vp[4];

	if (*effects[ei].e_init == NULL) {
		fprintf(stderr, "effect %d [\"%s\"] has no init method\n",
			ei, effects[ei].e_name);
		return;
	}

	darkness();
	glutPostRedisplay();
	
	/* deallocate previous effect */
	if (next_effect == -1)
		next_effect = 0;
	else {
		old = next_effect;
		(*effects[old].e_cleanup)(&effects[old]);
	}

	/* Restore default attributes.
	 * (check red book 2nd ed. p. 79)
	 * We do not want to restore the viewport size if
	 * it has changed.
	 */
	glGetIntegerv(GL_VIEWPORT, vp);
	glPopAttrib();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glViewport(vp[0], vp[1], vp[2], vp[3]);

	/* init frame count */
	messages_init_fps();

	/* allocate memory, do precomputations etc. */
	/* note: if the effect is not initialized correctly,
	 * we might get e.g. a segmentation fault here */
	err = (*effects[ei].e_init)(&effects[ei]);
	if (err < 0) {
		fprintf(stderr, "effect %d [\"%s\"] failed to initialize\n",
		       ei, effects[ei].e_name);
		exit(1);
	}

	glGetIntegerv(GL_VIEWPORT, vp);
	(*effects[ei].e_reshape)((int)vp[2], (int)vp[3]);

	next_effect = ei;
	
	flash();
}

void main_set_glut_callbacks(struct effect *ep, int w, int h)
{
	if (!ep) {
		fprintf(stderr, "main_set_glut_callbacks: ep NULL\n");
		exit(1);
	}

	glutDisplayFunc(ep->e_display);
	glutReshapeFunc(ep->e_reshape);
	glutKeyboardFunc(ep->e_keyboard);
	glutSpecialFunc(ep->e_special);
	glutMouseFunc(ep->e_mouse);
	glutMotionFunc(ep->e_motion);
	glutPassiveMotionFunc(ep->e_passive);
	glutIdleFunc(ep->e_idle);
	/* if we resized during pause */
	if (w != -1) {
		(ep->e_reshape)(w, h);
	}
	glutPostRedisplay();
}

/* 
 * After main_run_effect() and a flash() we can actually
 * call the main function of the next effect. It must
 * set all the appropriate glut callbacks.
 */
static void run_next_effect(void)
{
	int ei;
	struct effect *ep;

	ei = next_effect;
	ep = &effects[ei];

	/* set the effect as the current effect */
	current_effect = ep;

	/* set the glut callback, activating the new effect */
	main_set_glut_callbacks(ep, -1, -1);

	/* make sure we display the stuff */
	glutPostRedisplay();
}








/* ---------------------------------------------------------------------- */


static void flash_display_cb(void);
/* clear color */
static GLdouble cc;

static void flash(void)
{
	glutDisplayFunc(flash_display_cb);
	glutIdleFunc(main_idle_cb);
}

static int flash_init_ok = 0;
static GLfloat ccc[4];


static void flash_display_cb(void)
{
	/* let's not trash the clear color set by the effect */


	if (!flash_init_ok) {
		glGetFloatv(GL_COLOR_CLEAR_VALUE, ccc);

		cc = FLASH_INTENSITY_MAX;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 5.0,  /* eye */
			  0.0, 0.0, 0.0,  /* ctr */
			  0.0, 1.0, 0.0); /* up */
		flash_init_ok = 1;
	}

	glClearColor(cc, cc, cc, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cc -= FLASH_INTENSITY_DEC;

	if (cc < FLASH_INTENSITY_MIN) {
		flash_init_ok = 0;
		glClearColor(ccc[0], ccc[1], ccc[2], ccc[3]);
		run_next_effect();
	}

	glutSwapBuffers();
}


/* ---------------------------------------------------------------------- */


static void main_reshape_cb(int w, int h)
{
	printf("main_reshape_cb\n");
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 0.1, 100.0);
}

static void main_display_cb(void)
{
	printf("main_display_cb\n");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

static int main_run = 0;

static void main_idle_cb(void)
{
	if (!main_run) {
		main_run_effect(0);
		main_run = 1;
	}
	glutPostRedisplay();
}


static void darkness(void)
{
	printf("darkness func\n");
	glutDisplayFunc(main_display_cb);
	glutReshapeFunc(main_reshape_cb);
	glutIdleFunc(main_idle_cb);
	glutMouseFunc(NULL);
	glutKeyboardFunc(NULL);
	glutSpecialFunc(NULL);
}

/* ---------------------------------------------------------------------- */




int main(int argc, char *argv[])
{
	init_effects();
	srand(time(NULL));
	dummy = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(S_WIDTH, S_HEIGHT);
	glutInitWindowPosition(S_XPOS, S_YPOS);
	glutCreateWindow(S_APPNAME);

	/* store default attributes (redbook 2nd ed. p. 79) */
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glutReshapeFunc(main_reshape_cb);
	glutDisplayFunc(main_display_cb);
	glutIdleFunc(main_idle_cb);
	
	glutMainLoop();

	return 0;
}
