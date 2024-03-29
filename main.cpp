#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ball.cpp"
#include "pool.h"
#include "pool.cpp"
#include "pole.cpp"
#include "pole.h"
#include "porcupine.h"
#include "porcupine.cpp"
#include "ground.h"
#include "ground.cpp"
#include "timer.cpp"
#include "other_handlers.cpp"
#include "color.cpp"
#include "nonedit.cpp"
#include "input.cpp"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball;
vector <Ball> enemies(150);
Ground ground;
Pool pool;
Pole tPole1, tPole2;
Pole mPoleCenter, mPoleEnd1, mPoleEnd2;
Pool trampoline;
Porcupine porcupine;

int isJump = 0, isDrew=0, isFall = 0, doneJump, isCameraChange=0, score = 0, levels=10, currentLevel=1, penalty=0, limit=50, inPond=0, isMagnet=0, current_frame=0, endTime=0, start_time=0, vert=0, hor=0, inPorcupine=0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0, magnetX=0, magnetY=0, deltaX=10000,deltaY=10000;

Timer t60(1.0 / 60);

GLfloat vertex_buffer_data[] = {
        -0.08, -0.3, 0,
         0.08, -0.3, 0,
         0.08,  0.3, 0,
         0.08,  0.3, 0,
        -0.08,  0.3, 0,
        -0.08, -0.3, 0
};

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model
    // Scene render
    ball.draw(VP);
    pool.draw(VP);
    ground.draw(VP);
    trampoline.draw(VP);
    tPole1.draw(VP);
    tPole2.draw(VP);
    if(currentLevel==4)
        porcupine.draw(VP);
    if (isMagnet==1 && isDrew==1){
        mPoleEnd1.draw(VP);
        mPoleEnd2.draw(VP);
        mPoleCenter.draw(VP);
    }
    for (int i=0; i<enemies.size(); ++i)
        enemies[i].draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if(isDrew==0){
        if (inPond==1) {
            if (left)
                ball.pondMoveLeft(pool.radius);
            else if (right)
                ball.pondMoveRight(pool.radius);
            else if (up||space) {
                isJump = 1;
                isFall = 0;
                ball.speedy = 0.8;
                ball.jump();
            }
            else {
                if (ball.position.x<2)
                    ball.pondMoveRight(pool.radius);
                else if (ball.position.x>2)
                    ball.pondMoveLeft(pool.radius);
            }
        }
        else {
            if (left)
                ball.moveLeft();
            else if (right)
                ball.moveRight();
            else if (up||space) {
                isJump = 1;
                isFall = 0;
                ball.speedy = 0.1;
                ball.jump();
            }
        }
    }
    else {
        if (left)
            ball.moveLeft();
        else if (right)
            ball.moveRight();
        else if (up||space) {
            isJump = 1;
            isFall = 0;
            ball.speedy = 0.1;
            ball.jump();
        }
        else{
            float signx = ((magnetX-ball.position.x)*hor);
            float signy = ((magnetY-ball.position.y)*vert);
            deltaX = (signx<0)?0:(magnetX - ball.position.x);
            deltaY = (signy<0)?0:(magnetY - ball.position.y);
            ball.moveInMagneticField(deltaX, deltaY, 1.2f);
        }
    }
    return;
}

void displayScore() {
    char buffer[100];
    sprintf(buffer, "Pacman Killer Level: %d Score: %d",currentLevel-1, score);
    glfwSetWindowTitle(window,buffer);
}

void initLevel(){
    score = 0;
    penalty = currentLevel*3;
    limit = (currentLevel+1)*50;
    if (currentLevel==2){
        start_time = current_frame + rand()%60 + 120;
        isMagnet = 1;
    }
    else{
        isMagnet = 0;
        isDrew = 0;
    }
    currentLevel++;
}

void drawMagnet()
{
    GLfloat vertex_buffer_data2[] = {
            -0.3, -0.08, 0,
             0.3, -0.08, 0,
             0.3,  0.08, 0,
             0.3,  0.08, 0,
            -0.3,  0.08, 0,
            -0.3, -0.08, 0
    };
    float l = (ball.position.y<2)?(ball.position.y+2):(ball.position.y-2);
    magnetY = l + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4.0));
    magnetX = (rand()%2==0)?-3.5:3.5;
    float xshift = (magnetX<0)?0.2:-0.2;
    mPoleCenter  = Pole(magnetX, magnetY, COLOR_RED, vertex_buffer_data);
    mPoleEnd1    = Pole(magnetX+xshift, magnetY-0.3, COLOR_RED, vertex_buffer_data2);
    mPoleEnd2    = Pole(magnetX+xshift, magnetY+0.3, COLOR_RED, vertex_buffer_data2);
    return;
}

void tick_elements() {
    current_frame++;
    if (currentLevel==4)
        porcupine.move();
    if (isMagnet==1)
    {
        if (isDrew==1){
            if (current_frame==endTime){
                start_time = current_frame + rand()%60 + 120;
                isDrew=0;
                ball.speedx = 0.1;
                ball.speedy = 0.1;
                isJump=0;
                isFall=1;
            }
        }
        else {
            if (current_frame==start_time){
                isDrew=1;
                isJump=0;
                isFall=0;
                drawMagnet();
                hor = ((magnetX - ball.position.x)<0)?-1:1;
                vert = ((magnetY - ball.position.y)<0)?-1:1;
                endTime = current_frame + rand()%120 + 240;
            }
        }
    }
    if ((ball.position.x>=1.3) && (ball.position.x<=2.85) && (ball.position.y<=-2.9)){
        ball.speedx = 0.01;
        if (inPond==0)
            ball.adjustHeight(pool.radius);
        inPond = 1;
    }
    else{
        if(isDrew==0)
            ball.speedx = 0.1;
        if (isJump==0 && isFall==0 && isDrew==0)
            ball.position.y = -2.9;
        inPond = 0;
    }

    if ((ball.position.x>=-3.58) && (ball.position.x<=-2.42) &&ball.position.y<=-2.5){
        isJump = 1;
        isFall = 0;
        ball.speedy *= 1.35;
        ball.jump();
    }

    if(ball.position.y>=screen_center_y){
        isCameraChange=1;
        screen_center_y = ball.position.y;
    }
    else if(ball.position.y<=screen_center_y && ball.position.y>1){
        screen_center_y = ball.position.y;
    }
    else{
        if(isCameraChange==1){
            screen_center_y = (ball.position.y<-3)?0:(ball.position.y);
        }
    }
    reset_screen();
    for (int i=0; i <enemies.size(); ++i)
        enemies[i].moveRight();
    
    for (int i=0; i <enemies.size(); ++i)
    {
        if (detect_collision(ball.bounding_box(), enemies[i].bounding_box())) {
            if((ball.position.y)<=((enemies[i]).position.y)) {
                if(isFall==0) {
                    isFall = 1;
                    isJump = 0;
                }
                if (isDrew==1)
                    break;
                score -= penalty;
            }
            else {
                if(isFall==1&&isDrew!=1){
                    isFall = 0;
                    isJump = 1;
                }
                score += enemies[i].score;
                enemies.erase(enemies.begin() + i);
                if (isDrew!=1)
                    ball.speedy *= 1.2;
            }
            displayScore();
        }
    }
    if (currentLevel==4) {
        if (detect_collision(ball.bounding_box(), porcupine.bounding_box())){
            if(inPorcupine==0){
                inPorcupine=1;
                score -= 50;
                displayScore();   
            }
        }
        else {
            inPorcupine=0;
        }
    }
    for (int i=0; i <enemies.size(); ++i)
    {
        if(((enemies[i]).position.x - (enemies[i]).radius)>=4) {
            (enemies[i]).position.x = -4 - (enemies[i]).radius;
        }
    }
    return;
}

void createEnemies () {
    int i=0,j=0,k=0;
    srand(time(NULL));
    const color_t colorsArray[9] = { COLOR_GREEN,COLOR_RED,COLOR_DARKBLUE,COLOR_LIGHTBLUE,COLOR_ORANGE,COLOR_GOLD,COLOR_VIOLET,COLOR_BROWN,COLOR_GRAY};
    for (i=0; i<50; ++i)
    {
        int lowy = -2 + 2.5*i;
        j=0;
        while (j<3){
            int lowx = -4 + 2.5*j;
            float xcoord = lowx + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0));
            float ycoord = lowy + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(1.0)));
            float radius = 0.5 + static_cast <float> (rand()) /(static_cast <float> (RAND_MAX/(0.25)));
            int colorIndex = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/8));
            enemies[k++] = Ball(xcoord,ycoord,radius, colorsArray[colorIndex],0.01);
            j++;
        }
    }
    return;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    if (currentLevel==2)
    {
        cout << "Controls:\n";
        cout << "Left Arrow or Left Touchpad (move left)\n";
        cout << "Right Arrow or Right Touchpad (move right)\n";
        cout << "Up Arrow or Spacebar (jump)\n";
        cout << "Scroll in (zoom in) and scroll out (zoom out)\n";
    }

    screen_center_y = 0;
    ball        = Ball(-3.6, -2.9, 0.3f, COLOR_DARKBLUE, 0.1);
    ground      = Ground(-2, -5.5, COLOR_GREEN);
    pool        = Pool(2, -3.2, 0.8f, COLOR_LIGHTBLUE);
    trampoline  = Pool(-2.5, -2.7, 0.5f, COLOR_BROWN);
    tPole1      = Pole(-3.0, -2.9, COLOR_BROWN, vertex_buffer_data);
    tPole2      = Pole(-2.0, -2.9, COLOR_BROWN, vertex_buffer_data);
    
    if (currentLevel==4)
        porcupine   = Porcupine(-1, -3.2, COLOR_DARKBROWN);

    createEnemies();

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);
    displayScore();

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);
}

int main(int argc, char **argv) {

    /* Draw in loop */
    int i=0;
    int width  = 600;
    int height = 600;
    window = initGLFW(width, height);
    do
    {
        srand(time(0));
        initLevel();

        initGL (window, width, height);
        
        while (!glfwWindowShouldClose(window)) {
        // Process timers

            if (t60.processTick()) {
                // 60 fps
                // OpenGL Draw commands
                if (score>=limit)
                    break;
                draw();
                // Swap Frame Buffer in double buffering
                glfwSwapBuffers(window);
                tick_elements();
                if(isJump==1){
                    doneJump = ball.jump();
                    if(doneJump==0){
                        isJump=0;
                        isFall=1;
                    }
                }
                else if (isFall==1){
                    doneJump = ball.fall();
                    if(doneJump==0){
                        isFall = 0;
                        isJump = 0;
                        ball.position.y=-3;
                    }
                }
                tick_input(window);
            }
            // Poll for Keyboard and mouse events
            glfwPollEvents();
        }
        i++;
    } while(i<levels);
    quit(window);
}

void mouseMoveBall(double xoffset) {
    if (xoffset<0)
        ball.moveLeft();
    else if(xoffset>0)
        ball.moveRight();
    return;
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (sqrt(pow((a.x - b.x),2)+pow((a.y - b.y),2)) <= (a.width + b.width));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}