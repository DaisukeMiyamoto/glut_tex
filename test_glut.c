#include <stdio.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glut_wrapper.h"

typedef struct _Texture {
  unsigned int width;
  unsigned int height;
  unsigned char *img;
  unsigned char *img_back;
} Texture;

// local functions
static void setup_texture(Texture *tex);
void move_texture(void);

// Texture
static Texture _tex;


static void setup_texture(Texture *tex)
{
  const int TEX_BOX_SIZE = 32;
  const int TEX_SIZE = 128;
  int i, j;
  tex->width = TEX_SIZE;
  tex->height = TEX_SIZE;

  tex->img      = (unsigned char *)malloc(tex->width * tex->height * 4 * sizeof(unsigned char));
  tex->img_back = (unsigned char *)malloc(tex->width * tex->height * 4 * sizeof(unsigned char));

  for(i=0; i<tex->height; i++){
    for(j=0; j<tex->width; j++){
      if( (i / TEX_BOX_SIZE)&0x1 && (j / TEX_BOX_SIZE)&0x1 ){
	tex->img[4 * (i*tex->width + j) + 0] = 255;
	tex->img[4 * (i*tex->width + j) + 1] = 0;
	tex->img[4 * (i*tex->width + j) + 2] = 0;
	tex->img[4 * (i*tex->width + j) + 3] = 0;
      }else{
	tex->img[4 * (i*tex->width + j) + 0] = 0;
	tex->img[4 * (i*tex->width + j) + 1] = 0;
	tex->img[4 * (i*tex->width + j) + 2] = 0;
	tex->img[4 * (i*tex->width + j) + 3] = 0;
      }
    }
  }
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->img);

}


void move_texture(void)
{
  const int TEX_MOVE_SPEED = 5;
  Texture *tex = &_tex;
  int i, j;
  unsigned char *tmp;

  // swap
  tmp = tex->img;
  tex->img = tex->img_back;
  tex->img_back = tmp;

  // move + 1px
  for(i=0; i<tex->height; i++){
    for(j=0; j<tex->width; j++){
      tex->img[4 * (i*tex->width + j) + 0] = tex->img_back[4 * ( i * tex->width + (j-TEX_MOVE_SPEED)%tex->width) + 0];
      tex->img[4 * (i*tex->width + j) + 1] = tex->img_back[4 * ( i * tex->width + (j-TEX_MOVE_SPEED)%tex->width) + 1];
      tex->img[4 * (i*tex->width + j) + 2] = tex->img_back[4 * ( i * tex->width + (j-TEX_MOVE_SPEED)%tex->width) + 2];
      tex->img[4 * (i*tex->width + j) + 3] = tex->img_back[4 * ( i * tex->width + (j-TEX_MOVE_SPEED)%tex->width) + 3];
    }
  }
  glTexImage2D(GL_TEXTURE_2D, 0, 4, tex->width, tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->img);
}




int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glut_wrapper_init(argv[0]);

  setup_texture(&_tex);

  glut_wrapper_set_animation(move_texture);

  glutMainLoop();

  return(0);
}

