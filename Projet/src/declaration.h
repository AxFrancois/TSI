
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif

#include "glhelper.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "vec2.hpp"
#include "triangle_index.hpp"
#include "vertex_opengl.hpp"
#include "mesh.hpp"


//matrice de transformation
struct transformation
{
  vec3 rotation_euler;    // rotation autour de x, y, z
  vec3 rotation_center;
  vec3 translation;

  transformation():rotation_euler(),rotation_center(),translation(){}
};

struct camera
{
  transformation tr;
  mat4 projection;
};

struct objet
{
  GLuint prog;        // identifiant du shader
  GLuint vao;         // identifiant du vao
  GLuint nb_triangle; // nombre de triangle du maillage
  GLuint texture_id;  // identifiant de la texture
  bool visible;       // montre ou cache l'objet
};

struct objet3d : public objet
{
  transformation tr;
};

struct text : public objet
{
  std::string value;           // Value of the text to display
  vec2 bottomLeft;
  vec2 topRight;
};


void init_text(text *t);
void draw_text(const text* const t);


void initInfoPanel();
void initGrid();
void init_hold();

void draw_obj3d(const objet3d* const obj, camera cam);

void algorthmic_init();
const int size_height = 20;
const int size_width = 10;
const int next_size_height = 4;
const int next_size_width = 6;
void display_grid(int gridparam[size_height][size_width]);
void move_right();
void move_left();
void soft_drop();
void hard_drop();
void line_clear();
void rotate_right();
void rotate_left();

void update_display_grid();
void update_display_next_grid();

void generate_piece(int sort_piece);
void generate_next_piece(int sort_piece);

int generate_random_number_piece();


void generatePieceO();
void generatePieceI();
void generatePieceT();
void generatePieceL();
void generatePieceJ();
void generatePieceZ();
void generatePieceS();

void generateNextPieceO();
void generateNextPieceI();
void generateNextPieceT();
void generateNextPieceL();
void generateNextPieceJ();
void generateNextPieceZ();
void generateNextPieceS();

void reset_next_piece_grid();

void game_over();