#include "main.h"
#include "timer.h"
#include "ball.h"
#include "water.h"
#include "rock.h"
#include "bomb.h"
#include "monster.h"
#include "boss.h"
#include "gift.h"
#include "booster.h"
#include "coins.h"
#include "barrel.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Water sea;
Rock rocks1[20];
Bomb bomb1;
Monster monster1[10];
Boss boss1;
Gift gift1[10];
Booster booster1[10];
Coins coins1[20];
Barrel barrel1[10];

int view = 0;


int health=100;
int score = 0;

char scoreput[500];

int boss_flag = 0;

int boost_counter = 0;
int boost_flag = 0;
int wind_flag = 0;
int wind_counter = 0;
int space_key_flag = 0;
float p = 0.0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
float camera_x = 0, camera_y = 0, camera_z = 0;
float target_x = 0, target_y = 0, target_z = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 20, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!

    glm::vec3 eye (camera_x,camera_y,camera_z);
    glm::vec3 target(target_x,target_y,target_z);
    // glm::vec3 eye (ball1.position.x + 75*sin((ball1.rotation)*M_PI/180.0f), 45, ball1.position.z + 75*cos((ball1.rotation)*M_PI/180.0f));
    // glm::vec3 target (ball1.position.x, ball1.position.y, ball1.position.z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    sea.draw(VP);
    for(int i=0;i<20;i++){
      rocks1[i].draw(VP);
    }
    bomb1.draw(VP);
    for(int i=0;i<10;i++){
      monster1[i].draw(VP);
    }
    boss1.draw(VP);
    for(int i=0;i<10;i++){
      gift1[i].draw(VP);
    }
    if(boss_flag == 1){
      for(int i=0;i<10;i++){
        booster1[i].draw(VP);
      }
    }

    for(int i=0;i<20;i++){
      coins1[i].draw(VP);
    }

    for(int i=0;i<10;i++){
      barrel1[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int f_key = glfwGetKey(window, GLFW_KEY_F);
    int space_key = glfwGetKey(window, GLFW_KEY_SPACE);

    if (left) {
        ball1.rotation += 2;
        bomb1.rotation += 2;
    }
    else if (right) {
      ball1.rotation -= 2;
      bomb1.rotation -= 2;
    }
    if (up) {
      ball1.position.z -= cos((ball1.rotation)*M_PI/180.0f);
      ball1.position.x -= sin((ball1.rotation)*M_PI/180.0f);
      bomb1.position.z -= cos((ball1.rotation)*M_PI/180.0f);
      bomb1.position.x -= sin((ball1.rotation)*M_PI/180.0f);
    }
    else if (down) {
      ball1.position.z += cos((ball1.rotation)*M_PI/180.0f);
      ball1.position.x += sin((ball1.rotation)*M_PI/180.0f);
      bomb1.position.z += cos((ball1.rotation)*M_PI/180.0f);
      bomb1.position.x += sin((ball1.rotation)*M_PI/180.0f);
    }
    if (f_key) {
      bomb1.speedY = 3;
      bomb1.speed = sin((ball1.rotation)*M_PI/180.0f);
      bomb1.speedZ = cos((ball1.rotation)*M_PI/180.0f);
    }
    if (space_key){
      space_key_flag = 1;
      ball1.speedY = 1;
      bomb1.speedY = 1;
      bomb1.speed = 0;
      bomb1.speedZ = 0;
    }

}

void tick_elements() {
  speed_camera();
wind_counter++;
if(wind_counter>= 10000 && wind_counter<20000){
  wind_flag = 1;
}
if(wind_counter == 10000){
    p = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
else if(wind_counter < 10000){
  wind_flag = 0;
}
else if(wind_counter>=20000){
  wind_flag = 0;
  wind_counter = 0;
}

if(wind_flag == 1){
  ball1.tick();

  ball1.speed = p;//sin((ball1.rotation)*M_PI/180.0f);
  ball1.speedZ = p; //cos((ball1.rotation)*M_PI/180.0f);
}


  for(int i=0;i<20;i++){
    coins1[i].tick();
    if(detect_collision(ball1.bounding_box(), coins1[i].bounding_box())){
      coins1[i].set_position(0,500,0);
      score++;
      cout << score << "\n";
    }
  }
  boost_counter++;
  if(space_key_flag == 1){
    ball1.tick();
    if(ball1.position.y <= 0){
      space_key_flag = 0;
    }
  }
    if(boost_flag==1){
      ball1.tick();
      ball1.speed = sin((ball1.rotation)*M_PI/180.0f);
      ball1.speedZ = cos((ball1.rotation)*M_PI/180.0f);
  }
  if(boost_counter>500){
    boost_flag=0;
    boost_counter=0;
  }
    bomb1.tick();
    if(bomb1.position.y <= 7){
      bomb1.position.x = ball1.position.x;
      bomb1.position.z = ball1.position.z;
    }


    for(int i=0;i<20;i++){
      if(detect_collision(ball1.bounding_box(), rocks1[i].bounding_box())){
        ball1.set_position(ball1.position.x, ball1.position.y, ball1.position.z+4);
        health = health - 5;
        std::cout << health << '\n';
      }
    }


    for(int i=0;i<10;i++){
      if(detect_collision(ball1.bounding_box(), monster1[i].bounding_box())){
        ball1.set_position(ball1.position.x, ball1.position.y, ball1.position.z+4);
        health = health - 5;
        cout << health << "\n";
      }
    }


    for(int i=0;i<10;i++){
      if(detect_collision(ball1.bounding_box(), barrel1[i].bounding_box())){
        ball1.set_position(ball1.position.x, ball1.position.y, ball1.position.z+4);
      }
    }

    for(int i=0;i<10;i++){
      if(detect_collision(bomb1.bounding_box(), monster1[i].bounding_box())){

        bomb1.set_position(ball1.position.x, ball1.position.y+7, ball1.position.z);

        float x = (float)(-250 + rand() % 500);
        float z = (float)(-250 + rand() % 500);
        monster1[i].set_position(x, 50, z);
      }
    }

    if(detect_collision(ball1.bounding_box(), boss1.bounding_box())){
      ball1.set_position(ball1.position.x, ball1.position.y, ball1.position.z+4);
      health = health - 10;
      std::cout << health << '\n';
    }

    if(detect_collision(bomb1.bounding_box(), boss1.bounding_box())){
      bomb1.set_position(ball1.position.x, ball1.position.y+7, ball1.position.z+4);
      float x = (float)(-250 + rand() % 500);
      float z = (float)(-250 + rand() % 500);
      boss1.set_position(x, 50, z);
      boss_flag = 1;
    }

    for(int i=0;i<10;i++){
      if(detect_collision(ball1.bounding_box(), gift1[i].bounding_box())){
        gift1[i].set_position(0,-20,0);
        health += 10;
        cout << health << "\n";
      }
    }


    for(int i=0;i<10;i++){
      if(detect_collision(ball1.bounding_box(), booster1[i].bounding_box())){
        booster1[i].set_position(0,-20,0);
        boost_flag = 1;
      }
    }


  sprintf(scoreput,  "Score: %d         Health: %d", score, health);
  glfwSetWindowTitle(window,scoreput);


}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(0, 0, 0, COLOR_RED);

    sea         = Water(0, 0, 0, COLOR_BLUE);

    for(int i=0;i<20;i++){
      float x = (float)(-300 + rand() % 600);
      float z = (float)(-300 + rand() % 600);
      rocks1[i]      = Rock(x,0,z,COLOR_BROWN);
    }
    bomb1 = Bomb(ball1.position.x,ball1.position.y+7,ball1.position.z,COLOR_DARK_RED);

    for(int i=0;i<10;i++){
      float x = (float)(-300 + rand() % 600);
      float z = (float)(-300 + rand() % 600);
      monster1[i] = Monster(x,4,z,COLOR_GRAY);
    }

    boss1 = Boss(-125,0,120,COLOR_YELLOW);


    for(int i=0;i<10;i++){
      float x = (float)(-300 + rand() % 600);
      float z = (float)(-300 + rand() % 600);
      gift1[i] = Gift(x,4,z,COLOR_GIFT);
    }


    for(int i=0;i<10;i++){
      float x = (float)(-300 + rand() % 600);
      float z = (float)(-300 + rand() % 600);
      booster1[i] = Booster(x,4,z,COLOR_DARK_RED);
    }

    for(int i=0;i<20;i++){
      float x = (float)(-300 + rand() % 600);
      float z = (float)(-300 + rand() % 600);
      coins1[i] = Coins(x,4,z,COLOR_GOLDEN);
    }


    for(int i=0;i<10;i++){
      float x = (float)(-300 + rand() % 600);
      float z = (float)(-300 + rand() % 600);
      barrel1[i] = Barrel(x,4,z,COLOR_BARREL);
    }


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}



bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.length + b.length)) &&
    (abs(a.y - b.y) * 2 < (a.height + b.height)) &&
           (abs(a.z - b.z) * 2 < (a.width + b.width));
}

void reset_screen() {
//    float top    = screen_center_y + 4 / screen_zoom;
//    float bottom = screen_center_y - 4 / screen_zoom;
//    float left   = screen_center_x - 4 / screen_zoom;
//    float right  = screen_center_x + 4 / screen_zoom;
//    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    Matrices.projection = glm::perspective(45.0f,1.0f,10.0f,150.0f);
}


void camera_change()
{
    view = (view+1)%5;
}

void speed_camera()
{
//    cout << view << endl;
    if(view==0){
//follow
//        x_c=ball1.position.x+75*sin(ball1.rotation*M_PI/180.0f);
//        y_c=35;
//        z_c=ball1.position.z+75*cos(ball1.rotation*M_PI/180.0f);
        camera_x = ball1.position.x+75*sin(ball1.rotation*M_PI/180.0f);;
        camera_y = 35;
        camera_z = ball1.position.z+75*cos(ball1.rotation*M_PI/180.0f);
        target_x = ball1.position.x;
        target_y = ball1.position.y;
        target_z = ball1.position.z;

    }
    else if(view==1)
    {
        double theta = (ball1.rotation)*(M_PI/180);

        camera_x = ball1.position.x+3*sin(theta);
        camera_y = ball1.position.y+10;
        camera_z = ball1.position.z+3*cos(theta);

        target_x = ball1.position.x+10*sin(theta);
        target_y = ball1.position.y+10;
        target_z = ball1.position.z+10*cos(theta);

    }
    else if(view==2)
    {
        // tower view
        camera_y = 50;
        camera_z = 45;
//        camera_z+=5;
//        target_y = 30;
        target_x = ball1.position.x;
        target_y = ball1.position.y;
        target_z = ball1.position.z;
    }
    else if(view==3)
    {
        // top view
        camera_x = ball1.position.x;
        camera_y = 100;
        camera_z = ball1.position.z;

        target_x = ball1.position.x+1;
        target_y = ball1.position.y;
        target_z = ball1.position.z;
//        printf("cx=%f cy=%f cz=%f\n", camera_x, camera_y, camera_z);
//        printf("tx=%f ty=%f tz=%f\n", target_x, target_y, target_z);
    }
    else if(view==4)
    {
        // helicopter
        camera_x = ball1.position.x+90;
        camera_y = 90;
        camera_z = ball1.position.z;

        target_x = ball1.position.x;
        target_y = ball1.position.y;
        target_z = ball1.position.z;
    }

}

void heli_camera(float x, float y)
{
    if(view==2)
    {
        target_x = ball1.position.x+(x-300);
        if(y<=300)
        {
            target_y = ball1.position.y+(300-y)/2;
        }
    }
}

void zoom_camera(int type)
{
    if(view==2)
    {
        double l = target_x-camera_x;
        double m = target_y-camera_y;
        double n = target_z-camera_z;
        if(type==1)
        {
            if(camera_z-10>target_z)
                camera_z-=10;
        }
        else if(type==-1)
        {
            camera_z+=10;
        }
        camera_x = l*(camera_z-target_z)/n+target_x;
        camera_y = m*(camera_z-target_z)/n+target_y;
    }
}
