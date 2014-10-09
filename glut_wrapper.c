#include <stdio.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glut_wrapper.h"

struct {
  unsigned int show_fps:1;
  unsigned int move:1;
  unsigned int nframe;
} FLAGS;

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


// GLUT functions
void display_func(void);
void keyboard_func(unsigned char key, int x, int y);
void mouse_func(int button ,int state, int x, int y);
void motion_func(int x, int y);
void reshape_func(int width, int height);

void fpstimer(int value);
void movetimer(int value);

void reset_view(void);
void polarview(void);


// animation function pointer
static void (*_anmfunc)(void);


void glut_wrapper_init(char *progname, int window_width, int window_height)
{
  int width = window_width;
  int height = window_height;
  const float texcolor[] = { 1.0, 1.0, 1.0, 1.0 };

  // initialize valiables
  reset_view();
  FLAGS.nframe=0;
  FLAGS.show_fps = 0;
  FLAGS.move = 0;
  _anmfunc = NULL;

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
  glutTimerFunc(100, movetimer, 0);


  // setup OpenGL environment
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texcolor);

}


void glut_wrapper_set_animation(void (*anmfunc)(void))
{
  _anmfunc = anmfunc;
}


void display_func(void)
{
  FLAGS.nframe++;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  case 'M':
    FLAGS.move = ~FLAGS.move;
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
    printf("FPS : %d (%d, %d)\n", FLAGS.nframe - nframe_prev, FLAGS.nframe, nframe_prev);
  }
  nframe_prev = FLAGS.nframe;

  glutTimerFunc(1000, fpstimer, 0);
}

void movetimer(int value)
{
  if(FLAGS.move == 1){
    if(_anmfunc != NULL){
      _anmfunc();
      //move_texture(&_tex);
      
      glutPostRedisplay();
    }
  }
  glutTimerFunc(100, movetimer, 0);
}


void reset_view(void)
{
  CameraState.znear = 1.0;
  CameraState.zfar = 80.0;
  CameraState.fovy = 40.0;

  CameraState.distance = 4.0;
  CameraState.twist = 0.0;
  CameraState.elevation = 90.0;
  CameraState.azimuth = 0.0;
}

void polarview(void)
{
  glTranslatef( 0.0, 0.0, -CameraState.distance);
  glRotatef( -CameraState.twist, 0.0, 0.0, 1.0);
  glRotatef( -CameraState.elevation, 1.0, 0.0, 0.0);
  glRotatef( -CameraState.azimuth, 0.0, 1.0, 0.0);
}


