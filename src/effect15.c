#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "main.h"
#include "stars.h"

static char *effect15_name = "Hypnotic Patterns";

/* state variables */


static int frame_count = 0;      /* number of frames drawn */
static int freeze_tex = 0;
static int function = 0;

static GLfloat rot[3] = {0.0, 0.0, 0.0};
static GLfloat w[3] = {0.0, 0.0, 0.0};
#define MY_Z0 20.0
static GLfloat my_z = MY_Z0;


static GLfloat fontcolor[3] = {0.3, 1.0, 0.6};

/* texture width and height */
#define TW 128
#define TH 128

static GLubyte texture[TH][TW][3];
static GLuint  texname = 0;

#define NUMFUNCTIONS 2



static void effect15_reset(void)
{
	int i;
	
	for (i = 0; i < 3; i++) {
		rot[i] = 0.0;
		w[i] = 0.0;
	}
	my_z = MY_Z0;
}

#define F2(a, i, b, j, c, n, s) ((((a)*(i)+(b)*(j)+(c)*(n))>>(s))&0xff)

#define FF5(a5,a4,a3,a2,a1,b5,b4,b3,b2,b1,c5,c4,c3,c2,c1,d,D,s) \
(((((a1)*(I) + (a2)*(I2) + (a3)*(I3) + (a4)*(I4) + (a5)*(I5) \
  + (b1)*(J) + (b2)*(J2) + (b3)*(J3) + (b4)*(J4) + (b5)*(J5) \
  + (c1)*(n) + (c2)*(n2) + (c3)*(n3) + (c4)*(n4) + (c5)*(n5) + (d))/(D)) >>(s))&0xff)

#define FF4(a4,a3,a2,a1,b4,b3,b2,b1,c4,c3,c2,c1,d,D,s) \
FF5(0,a4,a3,a2,a1,0,b4,b3,b2,b1,0,c4,c3,c2,c1,d,D,s)

#define FF3(a3,a2,a1,b3,b2,b1,c3,c2,c1,d,D,s) \
FF4(0,a3,a2,a1,0,b3,b2,b1,0,c3,c2,c1,d,D,s)

#define FF2(a2,a1,b2,b1,c2,c1,d,D,s) \
FF3(0,a2,a1,0,b2,b1,0,c2,c1,d,D,s)

#define FF1(a1,b1,c1,d,D,s) \
FF2(0,a1,0,b1,0,c1,d,D,s)





static void make_texture0(GLubyte image[TH][TW][3], int n)
{
	int i, j, I, J;
	for (i = 0; i < TH; i++) {
		I = (i-TH/2)*(i-TH/2);
		for (j = 0; j < TW; j++) {
			J = (j-TW/2)*(j-TW/2);
			image[i][j][0] = F2(33, I, 92, J,-623,n,9);
			image[i][j][1] = F2(51, I, 21, J,-199,n,8);
			image[i][j][2] = F2(42, I,-51, J,  35,n,7);

		}
	}
}

static void make_texture1(GLubyte image[TH][TW][3], int n)
{
	int i, j, I, I2, I3, I4,I5;
	int J, J2, J3, J4, J5;
	int n2, n3, n4,n5;
	
	n2 = n * n; n3 = n * n2; n4 = n * n3; n5 = n * n4;
	for (i = 0; i < TH; i++) {
		I = (i-TH/2)*(i-TH/2);
		I2 = I*I; I3 = I*I2; I4 = I*I3; I5 = I*I4;
		
		for (j = 0; j < TW; j++) {
			J = (j-TW/2)*(j-TW/2);
			J2 = J*J; J3 = J*J2; J4 = J*J3; J5 = J*J4;
			
			image[i][j][0] = 
				(((I2 + i - n2)*(n - i)
				  /73322)>>23)
				 &0xff;

				 
			image[i][j][1] = 
			(((  (i+77)*(i-10)*(j+60)*(j-20)
			     +(i+98)*(j-12)
			     - (j+18)*(i+98)*(n+98))
			  /2172)>>21)&0xff;
			
			image[i][j][2] = 
			(((  		
				+(i+2)*(i-3) *(i+63)*(i+78)
				+(j-72)*(j-12)
				- (i+28)*(n+98)*(n+98))
			  /8387)>>23)&0xff;

		}
	}
	/* remove stupid irix cc warnings */
	I5=I5; J5=J5; n5=n5;
}








static void draw_cube(void)
{
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f( 0.5,  0.5,  0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5,  0.5,  0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5,  0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, -0.5,  0.5);

	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f( 0.5,  0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,  0.5,  0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f( 0.5, -0.5,  0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, -0.5, -0.5);

	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.5,  0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,  0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f( 0.5, -0.5, -0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);

	glNormal3f(-1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.5,  0.5,  0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5,  0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5,  0.5);

	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f( 0.5,  0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5,  0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5,  0.5,  0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f( 0.5,  0.5,  0.5);

	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f( 0.5, -0.5,  0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5,  0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, -0.5, -0.5);
	glEnd();

}





static void effect15_display_cb(void)
{
	int i;
	GLdouble a, b, fc, phi;
	char s[256];

	if ((frame_count % 1) == 0) {
		if (!freeze_tex) {
			switch (function) {
			case 0:
				make_texture0(texture, frame_count);
				break;
			case 1:
				make_texture1(texture, frame_count);
				break;
#if 0
			case 2:
				make_texture2(texture, frame_count);
				break;
			case 3:
				make_texture3(texture, frame_count);
				break;
			case 4:
				make_texture4(texture, frame_count);
				break;
			case 5:
				make_texture5(texture, frame_count);
				break;
#endif
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TW, TH, 
			     0, GL_RGB, GL_UNSIGNED_BYTE, texture);
		glBindTexture(GL_TEXTURE_2D, texname);
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glShadeModel(GL_FLAT);
	glDisable(GL_TEXTURE_2D);
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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



	stars_draw();  /* stars.h */

	glEnable(GL_TEXTURE_2D);


	fc = (GLdouble)frame_count;
	a = 7.0;
	b = 5.0;

	glPushMatrix();
	glRotatef(fc*0.4, 1.0, 0.0, 0.0);
	glRotatef(fc*0.5, 0.0, 1.0, 0.0);
	glRotatef(fc*0.6, 0.0, 0.0, 1.0);
	glScalef(3.0, 3.0, 3.0);
	draw_cube();
	glPopMatrix();

	for (i = 0; i < 10; i++) {
		glPushMatrix();
		phi = 20.0*(M_PI/180.0)*(GLdouble)i + fc/50.0;
		glTranslatef(a*cos(phi), b*sin(phi), 0.0);
		glRotatef(fc*1.0, 1.0, 0.0, 0.0);
		glRotatef(fc*2.0, 0.0, 1.0, 0.0);
		glRotatef(fc*3.0, 0.0, 0.0, 1.0);
		draw_cube();
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
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

static void effect15_reshape_cb(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble)w/(GLdouble)h, 0.1, 1000.0);
}

static void effect15_keyboard_cb(unsigned char key, int x, int y)
{
	x=x;y=y;
	switch (key) {
	case ' ':
		function = (function + 1) % NUMFUNCTIONS;
		break;
	case 's': /* status */
	case 'm':
		messages_toggle();
		break;
	case 'p':
		pause_request();
		break;
	case 'r':
		effect15_reset();
		break;
	case 'f':
		freeze_tex = !freeze_tex;
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
void effect15_special_cb(int key, int x, int y)
{
	x=x;y=y;
	switch (key) {
	case GLUT_KEY_PAGE_UP:
		my_z -= 2.0;
		if (my_z <= 0.0)
			my_z = 0.0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		my_z += 2.0;
		if (my_z >= STARS_MINDIST)
			my_z = STARS_MINDIST;
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




int effect15_init_cb(struct effect *ep)
{
	ep=ep;

#if 0
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
#endif

	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	frame_count = 0;

	return 0;
}



void effect15_cleanup_cb(struct effect *ep)
{
	(void)ep;
	glDeleteTextures(1, &texname);

	return;
}


int effect15_register(struct effect *ep)
{
	ep->e_init     = effect15_init_cb;
	ep->e_cleanup  = effect15_cleanup_cb;

	ep->e_display  = effect15_display_cb;
	ep->e_reshape  = effect15_reshape_cb;
	ep->e_keyboard = effect15_keyboard_cb;
	ep->e_special  = effect15_special_cb;
	ep->e_mouse    = NULL;

	ep->e_name     = effect15_name;
	return 0;
}
