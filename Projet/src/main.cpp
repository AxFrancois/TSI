/*****************************************************************************\
 * TP CPE, 4ETI, TP synthese d'images
 * --------------
 *
 * Programme principal des appels OpenGL
 \*****************************************************************************/

#include "declaration.h"

 /*****************************************************************************\
 * Globales                                                                    *
 \*****************************************************************************/
//identifiant des shaders
GLuint shader_program_id;
GLuint gui_program_id;

camera cam;

const int nb_obj = 201;
objet3d obj[nb_obj];

const int nb_text = 3;
text text_to_draw[nb_text];

float secondes = 0;
int timer = 0;


//const int size_height = 20;
//const int size_width = 10;

/*
Code num�ro pour la grille : 
https://i.stack.imgur.com/4pQum.png
0 - empty
1 - blue, immovable (I)
2 - yellow, immovable (O)
3 - purple, immovable (T)
4 - light orange, immovable (L)
5 - dark blue, immovable (J)
6 - orange, immovable (Z)
7 - green, immovable (S)
11 - blue (I)
12 - yellow (O)
13 - purple (T)
14 - light orange (L)
15 - dark blue (J)
16 - green (Z)
17 - green (S)
99 - test cube
*/
int grid[size_height][size_width] = {};

long int score = 0;

/*****************************************************************************\
* initialisation                                                              *
\*****************************************************************************/
static void init()
{
  shader_program_id = glhelper::create_program_from_file("shaders/shader.vert", "shaders/shader.frag"); CHECK_GL_ERROR();

  cam.projection = matrice_projection(60.0f*M_PI/180.0f,1.0f,0.01f,100.0f);
  cam.tr.translation = vec3(0.0f, 1.0f, 0.0f);
  // cam.tr.translation = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_center = vec3(0.0f, 20.0f, 0.0f);
  // cam.tr.rotation_euler = vec3(M_PI/2., 0.0f, 0.0f);

  initInfoPanel();
  initGrid();

  gui_program_id = glhelper::create_program_from_file("shaders/gui.vert", "shaders/gui.frag"); CHECK_GL_ERROR();


  text_to_draw[0].value = "Tetris";
  text_to_draw[0].bottomLeft = vec2(-0.9, 0.7);
  text_to_draw[0].topRight = vec2(-0.3, 1.2);

  init_text(text_to_draw);
  text_to_draw[1]=text_to_draw[0];

  char currentScore[20] = "Score : " ;
  char scoreBuffer[10];
  sprintf(scoreBuffer, "%d", score);
  strcat(currentScore, scoreBuffer);

  text_to_draw[1].value = currentScore;
  text_to_draw[1].bottomLeft = vec2(-0.9, 0.0);
  text_to_draw[1].topRight = vec2(-0.3, 0.5);

  char currentTime[20] = "Temps : ";
  char tempsBuffer[10];
  sprintf(tempsBuffer, "%d", timer);
  strcat(currentTime, tempsBuffer);
  text_to_draw[2] = text_to_draw[0];
  text_to_draw[2].value = tempsBuffer;
  text_to_draw[2].bottomLeft = vec2(-0.9, -0.5);
  text_to_draw[2].topRight = vec2(-0.3, 0.0);

  algorthmic_init();
}

static void algorthmic_init() {
    display_grid(grid);
    grid[0][0] = 11;
    grid[0][1] = 11;
    grid[0][2] = 11;
    grid[1][1] = 11;
    for (int j = 1; j < size_width; j++) {
        grid[19][j] = 1;
    }
    for (int j = 1; j < size_width; j++) {
        grid[18][j] = 1;
    }
    display_grid(grid);
}

/*****************************************************************************\
* display_callback                                                            *
\*****************************************************************************/
 static void display_callback()
{
  glClearColor(0.5f, 0.6f, 0.9f, 1.0f); CHECK_GL_ERROR();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); CHECK_GL_ERROR();

  for(int i = 0; i < nb_obj; ++i)
    draw_obj3d(obj + i, cam);

  for(int i = 0; i < nb_text; ++i)
    draw_text(text_to_draw + i);

  glutSwapBuffers();
}

/*****************************************************************************\
* keyboard_callback                                                           *
\*****************************************************************************/
static void keyboard_callback(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'p':
      glhelper::print_screen();
      break;
    case 'z':   //rotate left
    case 'Z':
        printf("Touche z\n");
        rotate_left();
        break;
    case 'c':   //HOLD (stock de pi�ce)
    case 'C':
        printf("Touche c\n");
        break;
    case ' ':   //Hard drop
        printf("Touche espace\n");
        hard_drop();
        break;
    case 27: //Touche Echap : gestion pause
      printf("Touche Echap\n");
      exit(0);
      break;
  }
}
// http://mperriss.free.fr/opengl/Guide_2D/claviersouris.htm

/*****************************************************************************\
* special_callback                                                            *
\*****************************************************************************/
static void special_callback(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: //Move left
        printf("Touche fleche gauche\n");
        move_left();
        break;
    case GLUT_KEY_UP: //rotate right
        printf("Touche fleche haut\n");
        rotate_right();
        break;
    case GLUT_KEY_RIGHT:    //move right
        printf("Touche fleche droite\n");
        move_right();
        break;
    case GLUT_KEY_DOWN: //soft drop
        printf("Touche fleche bas\n");
        soft_drop();
        break;
    default:
        break;
    }
}

/*****************************************************************************\
* Move functions                                                              *
\*****************************************************************************/
static void move_right()
{
    int AuthorizedMovement = 1;
    int new_grid[size_height][size_width]={};

    for (int i = 0; i < size_height; i++){
        for (int j = 0; j < size_width; j++){
            if (((0 <= grid[i][j]) && (grid[i][j] < 10)) && (new_grid[i][j] == 0)) { //if the cell contains immovable bloc and the target cell is empty, just copy the bloc where it is
                new_grid[i][j] = grid[i][j];
            }
            else if (grid[i][j] > 10) { //if the cell contains movable bloc
                if (j == size_width-1) {   //if we are at the border
                    AuthorizedMovement = 0;
                    break;
                }
                if ((0 < new_grid[i][j + 1] && new_grid[i][j + 1] < 10) || (0 < grid[i][j + 1] && grid[i][j + 1] < 10)) { //if the cell is already occupied by immovable object, unauthorized movement so we leave
                    AuthorizedMovement = 0;
                    break;
                }
                else {  //else we copy it with the right move
                    new_grid[i][j+1] = grid[i][j];
                }
            }
        }
        if (AuthorizedMovement == 0) {
            break;
        }
    }
    //Copy of new grid into old one if movement was correct
    if (AuthorizedMovement == 1) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }
    display_grid(grid);
}

static void move_left()
{
    int AuthorizedMovement = 1;
    int new_grid[size_height][size_width] = {};

    for (int i = 0; i < size_height; i++) {
        for (int j = 0; j < size_width; j++) {
            if (((0 <= grid[i][j]) && (grid[i][j] < 10)) && (new_grid[i][j] == 0)) { //if the cell contains immovable bloc and the target cell is empty, just copy the bloc where it is
                new_grid[i][j] = grid[i][j];
            }
            else if (grid[i][j] > 10) { //if the cell contains movable bloc
                if (j == 0) {   //if we are at the border
                    AuthorizedMovement = 0;
                    break;
                }
                if ((0 < new_grid[i][j - 1] && new_grid[i][j - 1] < 10) || (0 < grid[i][j - 1] && grid[i][j - 1] < 10)) { //if the cell is already occupied by immovable object, unauthorized movement so we leave
                    AuthorizedMovement = 0;
                    break;
                }
                else {  //else we copy it with the left move
                    new_grid[i][j - 1] = grid[i][j];
                }
            }
        }
        if (AuthorizedMovement == 0) {
            break;
        }
    }
    //Copy of new grid into old one if movement was correct
    if (AuthorizedMovement == 1) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }
    display_grid(grid);
}

static void soft_drop()
{
    int Colision = 0;
    int new_grid[size_height][size_width] = {};

    for (int i = 0; i < size_height; i++) {
        for (int j = 0; j < size_width; j++) {
            if (((0 <= grid[i][j]) && (grid[i][j] < 10)) && (new_grid[i][j] == 0)) { //if the cell contains immovable bloc and the target cell is empty, just copy the bloc where it is
                new_grid[i][j] = grid[i][j];
            }
            else if (grid[i][j] > 10) { //if the cell contains movable bloc
                if (i == size_height-1) {   //if we are at the bottom
                    Colision = 1;
                    break;
                }
                if ((0 < new_grid[i + 1][j] && new_grid[i + 1][j] < 10) || (0 < grid[i + 1][j] && grid[i + 1][j] < 10)) { //if the cell is already occupied by immovable object, there is colision !
                    Colision = 1;
                    break;
                }
                else {  //else we copy it with the down move
                    new_grid[i + 1][j] = grid[i][j];
                }
            }
        }
        if (Colision == 1) {
            break;
        }
    }

    //If there is colision : the piece can't move anymore
    if (Colision == 1) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                if (grid[i][j] > 10) { //if the cell contains movable bloc
                    int new_piece_value = 0;
                    switch (grid[i][j])
                    {
                        case 11:
                            new_piece_value = 1;
                            break;
                        case 12:
                            new_piece_value = 2;
                            break;
                        case 13:
                            new_piece_value = 3;
                            break;
                        case 14:
                            new_piece_value = 4;
                            break;
                        case 15:
                            new_piece_value = 5;
                            break;
                        case 16:
                            new_piece_value = 6;
                            break;
                        case 17:
                            new_piece_value = 7;
                            break;
                        default:
                            printf("soft_drop : Unexpected value, %d\n", grid[i][j]);
                            break;
                    }
                    grid[i][j] = new_piece_value;
                }
            }
        }
        line_clear();
        //GENERER UNE NOUVELLE PIECE
    }
    else {  //Copy of new grid into old one if there is no colision 
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }
    display_grid(grid);

}

static void hard_drop()
{
    for (int h = 0; h < size_height; h++) {
        soft_drop();
    }
}

/*****************************************************************************\
* Rotation functions                                                          *
\*****************************************************************************/

static void rotate_right()
{
    //top left corner and bottom right of the current piece in the grid. 
    int corner[4] = { size_height, size_width, 0, 0 };
    for (int i = 0; i < size_height; i++) {
        for (int j = 0; j < size_width; j++) {
            if (grid[i][j] > 10) {
                if (i < corner[0]) {
                    corner[0] = i;
                }
                if (j < corner[1]) {
                    corner[1] = j;
                }
                if (i > corner[2]) {
                    corner[2] = i;
                }
                if (j > corner[3]) {
                    corner[3] = j;
                }
            }
        }
    }
    //printf("%d %d %d %d\n", corner[0], corner[1], corner[2], corner[3]);

    int size_X = corner[2] - corner[0] + 1; 
    int size_Y = corner[3] - corner[1] + 1;
    //printf("%d %d\n", size_X, size_Y);

    int AuthorizedMovement = 1;
    int new_grid[size_height][size_width] = {};

    for (int i = 0; i < size_height; i++) {
        for (int j = 0; j < size_width; j++) {
            if (((0 <= grid[i][j]) && (grid[i][j] < 10)) && (new_grid[i][j] == 0)) { //if the cell contains immovable bloc and the target cell is empty, just copy the bloc where it is
                new_grid[i][j] = grid[i][j];
            }
            else if (grid[i][j] > 10) { //if the cell contains movable bloc
                int target_x = corner[0] + j - corner[1]; 
                int target_y = corner[1] + size_X - (i - corner[0]) -1;
                //printf("[%d,%d] -> [%d,%d]\n", i,j, target_x, target_y);
                //printf("%d %d\n", size_X, size_Y);
                //printf("%d %d\n", corner[0], corner[1]);
                if (target_y >= size_width || target_x >= size_height || target_y < 0 || target_x < 0) {   //if we are at the border
                    AuthorizedMovement = 0;
                    break;
                }
                if ((0 < new_grid[target_x][target_y] && new_grid[target_x][target_y] < 10) || (0 < grid[target_x][target_y] && grid[target_x][target_y] < 10)) { //if the cell is already occupied by immovable object, unauthorized movement so we leave
                    AuthorizedMovement = 0;
                    break;
                }
                else {  //else we copy it with the right move
                    new_grid[target_x][target_y] = grid[i][j];
                }
            }
        }
        if (AuthorizedMovement == 0) {
            break;
        }
    }
    //Copy of new grid into old one if movement was correct
    if (AuthorizedMovement == 1) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }
    display_grid(grid);
}

static void rotate_left()
{
    //top left corner and bottom right of the current piece in the grid. 
    int corner[4] = { size_height, size_width, 0, 0 };
    for (int i = 0; i < size_height; i++) {
        for (int j = 0; j < size_width; j++) {
            if (grid[i][j] > 10) {
                if (i < corner[0]) {
                    corner[0] = i;
                }
                if (j < corner[1]) {
                    corner[1] = j;
                }
                if (i > corner[2]) {
                    corner[2] = i;
                }
                if (j > corner[3]) {
                    corner[3] = j;
                }
            }
        }
    }
    //printf("%d %d %d %d\n", corner[0], corner[1], corner[2], corner[3]);

    int size_X = corner[2] - corner[0] + 1;
    int size_Y = corner[3] - corner[1] + 1;
    //printf("%d %d\n", size_X, size_Y);

    int AuthorizedMovement = 1;
    int new_grid[size_height][size_width] = {};

    for (int i = 0; i < size_height; i++) {
        for (int j = 0; j < size_width; j++) {
            if (((0 <= grid[i][j]) && (grid[i][j] < 10)) && (new_grid[i][j] == 0)) { //if the cell contains immovable bloc and the target cell is empty, just copy the bloc where it is
                new_grid[i][j] = grid[i][j];
            }
            else if (grid[i][j] > 10) { //if the cell contains movable bloc
                int target_x = j + corner[0] - corner[1]; //corner[0] + (j - corner[1]);
                int target_y = corner[1] + size_X - (i - corner[0]) - 1; //FONCTIONNE
                //printf("[%d,%d] -> [%d,%d]\n", i, j, target_x, target_y);
                //printf("%d %d\n", size_X, size_Y);
                //printf("%d %d\n", corner[0], corner[1]);
                if (target_y >= size_width || target_x >= size_height || target_y < 0 || target_x < 0) {   //if we are at the border
                    AuthorizedMovement = 0;
                    break;
                }
                if ((0 < new_grid[target_x][target_y] && new_grid[target_x][target_y] < 10) || (0 < grid[target_x][target_y] && grid[target_x][target_y] < 10)) { //if the cell is already occupied by immovable object, unauthorized movement so we leave
                    AuthorizedMovement = 0;
                    break;
                }
                else {  //else we copy it with the right move
                    new_grid[target_x][target_y] = grid[i][j];
                }
            }
        }
        if (AuthorizedMovement == 0) {
            break;
        }
    }
    //Copy of new grid into old one if movement was correct
    if (AuthorizedMovement == 1) {
        for (int i = 0; i < size_height; i++) {
            for (int j = 0; j < size_width; j++) {
                grid[i][j] = new_grid[i][j];
            }
        }
    }
    display_grid(grid);
}

/*****************************************************************************\
* Scoring functions                                                           *
\*****************************************************************************/

static void line_clear() 
{
    display_grid(grid);
    int lineClearCounter = 0;
    for (int i = 0; i < size_height; i++) {
        int hasEmpty = 0;
        for (int j = 0; j < size_width; j++) {
            if (grid[i][j] == 0) { 
                hasEmpty = 1; 
            }
        }
        if (hasEmpty == 0) {
            lineClearCounter++;
            for (int upperline = i; upperline >= 0; upperline--) {
                if (upperline > 0) {
                    for (int j = 0; j < size_width; j++) {
                        grid[upperline][j] = grid[upperline - 1][j];
                    }
                }
                else {
                    for (int j = 0; j < size_width; j++) {
                        grid[upperline][j] = 0;
                    }
                }
            }
        }
    }
    switch (lineClearCounter)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    case 4:
        score += 800;
        break;
    default:
        break;
    }
}

/*****************************************************************************\
* timer_callback                                                              *
\*****************************************************************************/
static void timer_callback(int)
{
  glutTimerFunc(25, timer_callback, 0);
  secondes += 0.025;
  if (timer+1 <= secondes)
  {
      timer += 1;
      char currentTime[20] = "Temps : ";
      char tempsBuffer[10];
      sprintf(tempsBuffer, "%d", timer);
      strcat(currentTime, tempsBuffer);
      text_to_draw[2] = text_to_draw[0];
      text_to_draw[2].value = currentTime;
      text_to_draw[2].bottomLeft = vec2(-0.9, -0.5);
      text_to_draw[2].topRight = vec2(-0.3, 0.0);

      
  }
  glutPostRedisplay();


}


/*****************************************************************************\
* main                                                                        *
\*****************************************************************************/
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(600, 600);
  glutCreateWindow("OpenGL");

  glutDisplayFunc(display_callback);
  glutKeyboardFunc(keyboard_callback);
  glutSpecialFunc(special_callback);
  glutTimerFunc(25, timer_callback, 0);

  glewExperimental = true;
  glewInit();

  std::cout << "OpenGL: " << (GLchar *)(glGetString(GL_VERSION)) << std::endl;

  init();
  glutMainLoop();

  return 0;
}

/*****************************************************************************\
* draw_text                                                                   *
\*****************************************************************************/
void draw_text(const text * const t)
{
  if(!t->visible) return;
  
  glDisable(GL_DEPTH_TEST);
  glUseProgram(t->prog);

  vec2 size = (t->topRight - t->bottomLeft) / float(t->value.size());
  
  GLint loc_size = glGetUniformLocation(gui_program_id, "size"); CHECK_GL_ERROR();
  if (loc_size == -1) std::cerr << "Pas de variable uniforme : size" << std::endl;
  glUniform2f(loc_size,size.x, size.y);     CHECK_GL_ERROR();

  glBindVertexArray(t->vao);                CHECK_GL_ERROR();
  
  for(unsigned i = 0; i < t->value.size(); ++i)
  {
    GLint loc_start = glGetUniformLocation(gui_program_id, "start"); CHECK_GL_ERROR();
    if (loc_start == -1) std::cerr << "Pas de variable uniforme : start" << std::endl;
    glUniform2f(loc_start,t->bottomLeft.x+i*size.x, t->bottomLeft.y);    CHECK_GL_ERROR();

    GLint loc_char = glGetUniformLocation(gui_program_id, "c"); CHECK_GL_ERROR();
    if (loc_char == -1) std::cerr << "Pas de variable uniforme : c" << std::endl;
    glUniform1i(loc_char, (int)t->value[i]);    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, t->texture_id);                            CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);                    CHECK_GL_ERROR();
  }
}

/*****************************************************************************\
* draw_obj3d                                                                  *
\*****************************************************************************/
void draw_obj3d(const objet3d* const obj, camera cam)
{
  if(!obj->visible) return;

  glEnable(GL_DEPTH_TEST);
  glUseProgram(obj->prog);
  
  {
    GLint loc_projection = glGetUniformLocation(shader_program_id, "projection"); CHECK_GL_ERROR();
    if (loc_projection == -1) std::cerr << "Pas de variable uniforme : projection" << std::endl;
    glUniformMatrix4fv(loc_projection,1,false,pointeur(cam.projection));    CHECK_GL_ERROR();

    GLint loc_rotation_view = glGetUniformLocation(shader_program_id, "rotation_view"); CHECK_GL_ERROR();
    if (loc_rotation_view == -1) std::cerr << "Pas de variable uniforme : rotation_view" << std::endl;
    mat4 rotation_x = matrice_rotation(cam.tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(cam.tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(cam.tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_view,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 cv = cam.tr.rotation_center;
    GLint loc_rotation_center_view = glGetUniformLocation(shader_program_id, "rotation_center_view"); CHECK_GL_ERROR();
    if (loc_rotation_center_view == -1) std::cerr << "Pas de variable uniforme : rotation_center_view" << std::endl;
    glUniform4f(loc_rotation_center_view , cv.x,cv.y,cv.z , 0.0f); CHECK_GL_ERROR();

    vec3 tv = cam.tr.translation;
    GLint loc_translation_view = glGetUniformLocation(shader_program_id, "translation_view"); CHECK_GL_ERROR();
    if (loc_translation_view == -1) std::cerr << "Pas de variable uniforme : translation_view" << std::endl;
    glUniform4f(loc_translation_view , tv.x,tv.y,tv.z , 0.0f); CHECK_GL_ERROR();
  }
  {
    GLint loc_rotation_model = glGetUniformLocation(obj->prog, "rotation_model"); CHECK_GL_ERROR();
    if (loc_rotation_model == -1) std::cerr << "Pas de variable uniforme : rotation_model" << std::endl;
    mat4 rotation_x = matrice_rotation(obj->tr.rotation_euler.x, 1.0f, 0.0f, 0.0f);
    mat4 rotation_y = matrice_rotation(obj->tr.rotation_euler.y, 0.0f, 1.0f, 0.0f);
    mat4 rotation_z = matrice_rotation(obj->tr.rotation_euler.z, 0.0f, 0.0f, 1.0f);
    glUniformMatrix4fv(loc_rotation_model,1,false,pointeur(rotation_x*rotation_y*rotation_z));    CHECK_GL_ERROR();

    vec3 c = obj->tr.rotation_center;
    GLint loc_rotation_center_model = glGetUniformLocation(obj->prog, "rotation_center_model");   CHECK_GL_ERROR();
    if (loc_rotation_center_model == -1) std::cerr << "Pas de variable uniforme : rotation_center_model" << std::endl;
    glUniform4f(loc_rotation_center_model , c.x,c.y,c.z , 0.0f);                                  CHECK_GL_ERROR();

    vec3 t = obj->tr.translation;
    GLint loc_translation_model = glGetUniformLocation(obj->prog, "translation_model"); CHECK_GL_ERROR();
    if (loc_translation_model == -1) std::cerr << "Pas de variable uniforme : translation_model" << std::endl;
    glUniform4f(loc_translation_model , t.x,t.y,t.z , 0.0f);                                     CHECK_GL_ERROR();
  }
  glBindVertexArray(obj->vao);                                              CHECK_GL_ERROR();

  glBindTexture(GL_TEXTURE_2D, obj->texture_id);                            CHECK_GL_ERROR();
  glDrawElements(GL_TRIANGLES, 3*obj->nb_triangle, GL_UNSIGNED_INT, 0);     CHECK_GL_ERROR();
}

void init_text(text *t){
  vec3 p0=vec3( 0.0f, 0.0f, 0.0f);
  vec3 p1=vec3( 0.0f, 1.0f, 0.0f);
  vec3 p2=vec3( 1.0f, 1.0f, 0.0f);
  vec3 p3=vec3( 1.0f, 0.0f, 0.0f);

  vec3 geometrie[4] = {p0, p1, p2, p3}; 
  triangle_index index[2] = { triangle_index(0, 1, 2), triangle_index(0, 2, 3)};

  glGenVertexArrays(1, &(t->vao));                                              CHECK_GL_ERROR();
  glBindVertexArray(t->vao);                                                  CHECK_GL_ERROR();

  GLuint vbo;
  glGenBuffers(1, &vbo);                                                       CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo);                                          CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,sizeof(geometrie),geometrie,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); CHECK_GL_ERROR();

  GLuint vboi;
  glGenBuffers(1,&vboi);                                                      CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi);                                 CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);   CHECK_GL_ERROR();

  t->texture_id = glhelper::load_texture("data/fontB.tga");

  t->visible = true;
  t->prog = gui_program_id;
}

GLuint upload_mesh_to_gpu(const mesh& m)
{
  GLuint vao, vbo, vboi;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);                                 CHECK_GL_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER,vbo); CHECK_GL_ERROR();
  glBufferData(GL_ARRAY_BUFFER,m.vertex.size()*sizeof(vertex_opengl),&m.vertex[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  glEnableVertexAttribArray(0); CHECK_GL_ERROR();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), 0); CHECK_GL_ERROR();

  glEnableVertexAttribArray(1); CHECK_GL_ERROR();
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_opengl), (void*)sizeof(vec3)); CHECK_GL_ERROR();

  glEnableVertexAttribArray(2); CHECK_GL_ERROR();
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(2*sizeof(vec3))); CHECK_GL_ERROR();

  glEnableVertexAttribArray(3); CHECK_GL_ERROR();
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_opengl), (void*)(3*sizeof(vec3))); CHECK_GL_ERROR();

  glGenBuffers(1,&vboi); CHECK_GL_ERROR();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vboi); CHECK_GL_ERROR();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,m.connectivity.size()*sizeof(triangle_index),&m.connectivity[0],GL_STATIC_DRAW); CHECK_GL_ERROR();

  return vao;
}

void initInfoPanel()
{

  mesh m;

  //coordonnees geometriques des sommets
  vec3 p0=vec3(-2.9f,-2.0f,-5.5f);
  vec3 p1=vec3( -0.9f,-2.0f,-5.5f);
  vec3 p2=vec3( -2.9f,4.0f,-5.5f);
  vec3 p3=vec3(-0.9f,4.0f,-5.5f);

  //normales pour chaque sommet
  vec3 n0=vec3(0.0f,0.0f,1.0f);
  vec3 n1=n0;
  vec3 n2=n0;
  vec3 n3=n0;

  //couleur pour chaque sommet
  vec3 c0=vec3(1.0f,1.0f,1.0f);
  vec3 c1=c0;
  vec3 c2=c0;
  vec3 c3=c0;

  //texture du sommet
  vec2 t0=vec2(0.0f,0.0f);
  vec2 t1=vec2(1.0f,0.0f);
  vec2 t2=vec2(1.0f,1.0f);
  vec2 t3=vec2(0.0f,1.0f);

  vertex_opengl v0=vertex_opengl(p0,n0,c0,t0);
  vertex_opengl v1=vertex_opengl(p1,n1,c1,t1);
  vertex_opengl v2=vertex_opengl(p2,n2,c2,t2);
  vertex_opengl v3=vertex_opengl(p3,n3,c3,t3);

  m.vertex = {v0, v1, v2, v3};

  //indice des triangles
  triangle_index tri0=triangle_index(0,1,2);
  triangle_index tri1=triangle_index(1,3,2);  
  m.connectivity = {tri0, tri1};

  obj[0].nb_triangle = 2;
  obj[0].vao = upload_mesh_to_gpu(m);

  obj[0].texture_id = glhelper::load_texture("data/white.tga");

  obj[0].visible = true;
  obj[0].prog = shader_program_id;
}

void initGrid()
{
  // Chargement d'un maillage a partir d'un fichier
  mesh m = load_obj_file("data/untitled.obj");

    // Affecte une transformation sur les sommets du maillage
  float s = 0.15f;
  float sizeOfOneCube = s * 2;
  mat4 transform = mat4(   s, 0.0f, 0.0f, 0.0f,
      0.0f,    s, 0.0f, 0.50f,
      0.0f, 0.0f,   s , 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f);
  apply_deformation(&m,matrice_rotation(M_PI/2.0f,1.0f,0.0f,0.0f));
  apply_deformation(&m,matrice_rotation(M_PI,0.0f,1.0f,0.0f));
  apply_deformation(&m,transform);

  // Centre la rotation du modele 1 autour de son centre de gravite approximatif
  obj[1].tr.rotation_center = vec3(0.0f,0.0f,0.0f);

  update_normals(&m);
  fill_color(&m,vec3(1.0f,1.0f,1.0f));

  obj[1].vao = upload_mesh_to_gpu(m);

  obj[1].nb_triangle = m.connectivity.size();
  obj[1].texture_id = glhelper::load_texture("data/blue.tga");

  obj[1].visible = true;
  obj[1].prog = shader_program_id;

  obj[1].tr.translation = vec3(-0.5, -2.3, -5.5);

  int currentRows = 0;
  int currentColumns = 1;
  for(int i = 1;i<200;i++)
  {
    obj[i+1] = obj[1];

   if (i % 10 == 0)
    {
      ++currentRows;
      currentColumns = 0;
    }

    obj[i+1].tr.translation.x += sizeOfOneCube *currentColumns;
    obj[i+1].tr.translation.y += sizeOfOneCube *currentRows;
    ++currentColumns;
  }
}

/*****************************************************************************\
* Debug funtions                                                              *
\*****************************************************************************/

/*
grid, size_height and size_width are global values, therefore we don't need parameters.
print the grid in the terminal.
 */
void display_grid(int gridparam[size_height][size_width]) {
    for (int i = 0; i < size_height; i++)
    {
        printf("[");
        for (int j = 0; j < size_width; j++)
        {
            printf("%d", gridparam[i][j]);
            if (j != size_width - 1) {
                printf(" ");
            }
        }
        printf("]\n");
    }
    printf("\n");
}