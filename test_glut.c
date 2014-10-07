#include <stdio.h>
#include <stdio.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "test_glut.h"


struct {
  int pos[2];
  int button;
} MouseState;

struct {
  float distance;
  float twist;
  float elevation;
  float azimuth;
  float znear;
  float zfar;
  float fovy;
} CameraState;

struct {
  unsigned int show_fps:1;
} FLAGS;

// GLUT functions
void display_func(void);
void keyboard_func(unsigned char key, int x, int y);
void mouse_func(int button ,int state, int x, int y);
void motion_func(int x, int y);
void fpstimer(int value);


// local functions
static void setup_texture(Texture *tex);
static void reset_view(void);
static void polarview(void);
static void init (char *progname);


// Texture
Texture _tex;

// FPS counter
unsigned int nframe = 0;

void display_func(void)
{
  const float texcolor[] = { 1.0, 1.0, 1.0, 1.0 };

  nframe++;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texcolor);
  glEnable(GL_TEXTURE_2D);


  glPushMatrix ();
  {
    polarview();    
    glEnable( GL_DEPTH_TEST );

    glNormal3f(0.0,1.0,0.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    {
      glTexCoord2f(0.0,0.0); glVertex3f(1.0,0.0,1.0);
      glTexCoord2f(0.0,5.0); glVertex3f(1.0,0.0,-1.0);
      glTexCoord2f(5.0,5.0); glVertex3f(-1.0,0.0,-1.0);
      glTexCoord2f(5.0,0.0); glVertex3f(-1.0,0.0,1.0);
    }
    glEnd();
    glDisable(GL_DEPTH_TEST);
  }
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

  glutSwapBuffers(); 

}

void keyboard_func(unsigned char key, int x, int y)
{
  switch(key) {
  case 'R':
    reset_view();
    break;
  case 'T':
    //reset_texture();
    break;
  case 'F':
    FLAGS.show_fps = ~FLAGS.show_fps;
    break;
  case KEY_ESC:
    exit( 0 );
  default :
    break;
  }
  glutPostRedisplay();
}

void mouse_func(int button ,int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    MouseState.pos[0] = x;
    MouseState.pos[1] = y;
    MouseState.button = button;
  }
}

void motion_func(int x, int y)
{
  int x_move, y_move;

  x_move = x - MouseState.pos[0];
  y_move = y - MouseState.pos[1];
  switch(MouseState.button){
  case GLUT_LEFT_BUTTON:
    CameraState.azimuth += (float) x_move/2.0;
    CameraState.elevation -= (float) y_move/2.0;
    break;
  case GLUT_MIDDLE_BUTTON:
    CameraState.twist = fmod (CameraState.twist + x_move/3.0, 360.0);
    break;
  case GLUT_RIGHT_BUTTON:
    CameraState.distance -= (float) y_move/60.0;
    break;
  }
  MouseState.pos[0] = x;
  MouseState.pos[1] = y;
  glutPostRedisplay();
}


void reshape_func(int width, int height)
{
  float aspect;

  aspect = (float) width / (float) height;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(CameraState.fovy, aspect, CameraState.znear, CameraState.zfar);
  glMatrixMode(GL_MODELVIEW);
}

void fpstimer(int value)
{
  static unsigned int nframe_prev=0;
  if(FLAGS.show_fps == 1){
    printf("FPS : %d (%d, %d)\n", nframe - nframe_prev, nframe, nframe_prev);
  }
  nframe_prev = nframe;

  glutTimerFunc(1000, fpstimer, 0);
}


static void setup_texture(Texture *tex)
{
  const int TEX_BOX_SIZE = 32;
  int i, j;
  tex->width = 128;
  tex->height = 128;

  tex->img = (unsigned char *)malloc(tex->width * tex->height * 4 * sizeof(unsigned char));

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

static void reset_view(void)
{
  CameraState.znear = 1.0;
  CameraState.zfar = 80.0;
  CameraState.fovy = 40.0;

  CameraState.distance = 4.0;
  CameraState.twist = 0.0;
  CameraState.elevation = 90.0;
  CameraState.azimuth = 0.0;
}

static void polarview(void)
{
  glTranslatef( 0.0, 0.0, -CameraState.distance);
  glRotatef( -CameraState.twist, 0.0, 0.0, 1.0);
  glRotatef( -CameraState.elevation, 1.0, 0.0, 0.0);
  glRotatef( -CameraState.azimuth, 0.0, 1.0, 0.0);
}


static void init (char *progname)
{
  int width = 800, height = 800;

  // initialize valiables
  reset_view();
  FLAGS.show_fps = 0;

  // setup window
  glutInitWindowPosition(0, 0);
  glutInitWindowSize( width, height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow(progname);
  glClearColor (0.0, 0.0, 0.0, 1.0);

  // setup glut callback funcs
  glutKeyboardFunc(keyboard_func);
  glutMouseFunc(mouse_func);
  glutMotionFunc(motion_func);
  glutReshapeFunc (reshape_func);
  glutDisplayFunc(display_func);
  glutTimerFunc(1000, fpstimer, 0);


  // setup OpenGL environment
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  setup_texture(&_tex);

}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  init(argv[0]);

  glutMainLoop();

  return(0);
}

