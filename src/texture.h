#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include "util/readtex.h"

typedef struct texture {
	GLuint      tx_name;    /* texture name (from glGenTextures) */
	GLint       tx_w;       /* width in pixels */
	GLint       tx_h;       /* height in pixels */
	GLenum      tx_fmt;     /* format: RGB or RGBA */
	GLubyte    *tx_data;    /* image data */
	char       *tx_filename;/* */
} texture_t;



texture_t *tex_init(char *filename);
void tex_fmt_rgb_to_rgba(texture_t *tp, GLubyte alpha);
void tex_free(texture_t *tp);
void tex_use(texture_t *tp);

#if 0
extern __inline void tex_use(texture_t *tp)
{
	if (!tp || !tp->tx_name)
		return;
	glBindTexture(GL_TEXTURE_2D, tp->tx_name);
}
#endif


#endif /* TEXTURE_H */

