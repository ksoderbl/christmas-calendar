#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/gl.h>
#include "main.h"
#include "util/readtex.h"
#include "texture.h"

texture_t *tex_init(char *filename)
{
	texture_t *p;
	size_t size = sizeof(texture_t);

	if (!filename) {
		fprintf(stderr, "tex_init: no texture file name given\n");
		return NULL;
	}

	/*printf("strlen(\"%s\") = %d\n", filename, strlen(filename));*/

	p = malloc(size);
	if (!p) {
		fprintf(stderr, "tex_init: could not alloc tex struct\n");
		return NULL;
	}
	memset(p, 0, size);

	p->tx_filename = malloc(strlen(filename) + 1);
	strcpy(p->tx_filename, filename);
	if (!p->tx_filename) {
		fprintf(stderr, "tex_init: could not alloc filename string\n");
		goto fail;
	}

	p->tx_data = LoadRGBImage(p->tx_filename,
				  &p->tx_w, &p->tx_h, &p->tx_fmt);
	if (!p->tx_data) {
		fprintf(stderr, "tex_init: could not find file %s\n",
			filename);
		goto fail;
	}

	glGenTextures(1, &p->tx_name);
	printf("got texture name %d for %s\n",(int)p->tx_name, p->tx_filename);
	glBindTexture(GL_TEXTURE_2D, p->tx_name);
	if (!glIsTexture(p->tx_name)) {
		fprintf(stderr, "tex_init: failed to bind texture\n");
		goto fail;
	}

	return p;
 fail:
	tex_free(p);
	return NULL;
}



void tex_use(texture_t *tp)
{
	if (!tp || !tp->tx_name)
		return;
	glBindTexture(GL_TEXTURE_2D, tp->tx_name);
}



void tex_fmt_rgb_to_rgba(texture_t *tx, GLubyte alpha)
{
	GLubyte *newdata, *p;
	int i;
	size_t npixels;
	alpha = alpha;

	printf("doing tex_rgb_to_rgba(%p)\n", tx);
	
	if (!tx)
		return;
	if (tx->tx_fmt != GL_RGB || tx->tx_fmt == GL_RGBA) {
		fprintf(stderr, "tex_rgb_to_rgba - fmt error\n");
		return;
	}

	npixels = tx->tx_w * tx->tx_h;
	newdata = malloc(npixels * 4);
	if (!newdata) {
		fprintf(stderr, "could not alloc tex struct\n");
		exit(1);
	}
	memset(newdata, 0, npixels * 4);
	p = tx->tx_data;
	
	for (i = 0; i < npixels; i++) {
		newdata[4*i + 0] = *p++;  /* r */
		newdata[4*i + 1] = *p++;  /* g */
		newdata[4*i + 2] = *p++;  /* b */
		newdata[4*i + 3] = /*alpha */*(p-1); /* a */
	}

	free(tx->tx_data);
	tx->tx_data = newdata;
	tx->tx_fmt  = GL_RGBA;
}


void tex_free(texture_t *tx)
{
	if (!tx)
		return;
	printf("doing tex_free(%p)\n", tx->tx_filename);

	printf("is texture (%d) = %d\n",
		  (int)tx->tx_name, glIsTexture(tx->tx_name));
	if (tx->tx_name)
		glDeleteTextures(1, &tx->tx_name);
	printf("is texture (%d) = %d\n",
		  (int)tx->tx_name, glIsTexture(tx->tx_name));

	if (tx->tx_data)
		free(tx->tx_data);
	if (tx->tx_filename)
		free(tx->tx_filename);
	free(tx);
}
