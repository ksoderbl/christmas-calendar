#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include "main.h"

/*
 * This pausing can be used by effects to freeze their screens.
 * It should take care of other glut callbacks also, but who
 * cares.
 */

static int pausing = 0;
static int reshape_width = -1;
static int reshape_height = -1;

static void pause_display_cb(void);
static void pause_keyboard_cb(unsigned char key, int x, int y);
static void pause_reshape_cb(int width, int height);


void pause_request(void)
{
	printf("pause_request\n");
	pausing = 1;
}

void pause_activate(void)
{
	struct effect e;

	printf("pausing...\n");

	reshape_width = -1;
	reshape_height = -1;

	memset(&e, 0, sizeof(struct effect));
	e.e_display = pause_display_cb;
	e.e_reshape = pause_reshape_cb;
	e.e_keyboard = pause_keyboard_cb;

	main_set_glut_callbacks(&e, -1, -1);
}

int pause_is_requested(void)
{
	return pausing;
}



static void pause_display_cb(void)
{
	/* do nothing */
	return;
}

static void pause_keyboard_cb(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
	
	printf("pause keyboard cb\n");

	switch (key) {
		/* quit pausing */
	case ' ':
	case 'p':
	case ASCII_ESC:
		pausing = 0;
		printf("current effect is %s\n", current_effect->e_name);
		main_set_glut_callbacks(current_effect, 
					reshape_width, reshape_height);
		break;
	default:
		break;
	}
}

static void pause_reshape_cb(int width, int height)
{
	reshape_width = width;
	reshape_height = height;

	printf("reshape called during pause ...\n");
}
