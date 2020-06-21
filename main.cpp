#include <GL/glut.h>
#include <math.h>
#include "glm.h"
#include <stdio.h>
#include <iostream>

#include "imageloader.h"

static int shoulder = 0, shoulder2_1 = 0, shoulder2_2 = 0, elbow = 0, finger1Base = 0, finger1Up = 0, finger2Base = 0, finger2Up = 0,
        finger3Base = 0, finger3Up = 0, finger4Base = 0, finger4Up = 0, rhip = 0, rhip2 = 0, rknee = 0, lknee = 0,
        lhip = 0, lhip2 = 0, hand_up = 0, around = 0;

static float Y = 0.02;


// poses of walking
int poses1 [][7] = {
        // shoulder2_1, shoulder2_2, elbow, rhip, rknee, lhip, lknee
        {0,0,0,0,0,0,0},
        {-20,20,-55,-25,-10,-30,10},
        {0,0,-55,-5,5,-5,-5},
        {25,-15,-55,15,10,15,-20},
        {30,-25,-55,25,10,15,-20},
        {5,-5,-55,10,0,10,0}
};

// poses of jumping
float poses2 [][6] = {
        // lknee , rknee , shoulder2_1, shoulder2_2, elbow, Y
        {-20, 20, 0,0,0,0.02},
        {0,0,-45,-45,-45,0.028},
        {0,0,-90,-90,-30,0.032},
        {0,0,-150,-150,-20,0.076},
        {0,0,-180,-180,-10,0.090},
        {0,0,-150,-150,-20,0.076},
        {0,0,-90,-90,-30,0.032},
        {0,0,-45,-45,-45,0.02},
        {0, 0, 0,0,0,0.02},
};

static int f1 = 0;
static int f2 = 0;

void setPoses(int frame, int num){
    switch (num) {
        case 1:
            shoulder2_1 = poses1[frame][0];
            shoulder2_2 = poses1[frame][1];
            elbow = poses1[frame][2];
            rhip = poses1[frame][3];
            rknee = poses1[frame][4];
            lhip = poses1[frame][5];
            lknee = poses1[frame][6];
            break;
        case 2 :
            lknee = poses2[frame][0];
            rknee = poses2[frame][1];
            shoulder2_1 = poses2[frame][2];
            shoulder2_2 = poses2[frame][3];
            elbow = poses2[frame][4];
            Y = poses2[frame][5];
            break;
    }
}

void walk(int value) {
    f1 = f1 % 6;
    setPoses(f1, 1);
    f1++;
    glutPostRedisplay();
    glutTimerFunc(150, walk, 0);
}

void jump(int value) {
    f2 = f2 % 9;
    setPoses(f2, 2);
    f2++;
    glutPostRedisplay();
    glutTimerFunc(150, jump, 0);
}

double eye[] = {0, 0, 0};
double center[] = {0, 0, -1};
double up[] = {0, 1, 0};
double horizontal_dir[] = {0, 0, 0};
double look_dir[] = {0, 0,};
double direction[] = {0, 0, 0};
float speed = 0.05;

//1
int windowWidth = 1024;
int windowHeight = 768;
float aspect = float(windowWidth) / float(windowHeight);

float DRot = 90;
float Zmax, Zmin;
GLMmodel *pmodel;
GLMmodel *pmodel2;
GLMmodel *pmodel3;
GLMmodel *pmodel4;
GLMmodel *pmodel5;
float VRot = 0.0;


// RGBA
GLfloat light_ambient[] = {0.0, 0.0, 0.0, 0.0};
GLfloat light_diffuse[] = {0.5, 0.5, 0.5, 1.0};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
// x , y, z, w
GLfloat light_position[] = {0.5, 5.0, 0.0, 1.0};
GLfloat lightPos1[] = {-0.5, -5.0, -2.0, 1.0};
// Material Properties
GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0};
GLfloat mat_specular[] = {0.0, 0.0, 0.0, 1.0};
GLfloat shininess[] = {100.0};
int moving, startx, starty;
//1

GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;

void init(void) {
    glMatrixMode(GL_PROJECTION);
    gluPerspective(65.0, (GLfloat) 1024 / (GLfloat) 869, 1.0, 60.0);
}

void crossProduct(double a[], double b[], double c[]) {
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[]) {
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[]) {

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta) * p[0];
    temp[1] += cos(theta) * p[1];
    temp[2] += cos(theta) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];

}

void Left() {
    double speed = 3.1415 / 100;
    double direction[] = {0, 1, 0};
    rotatePoint(direction, speed, center);
}

void Right() {
    double speed = -3.1415 / 100;
    double direction[] = {0, 1, 0};
    rotatePoint(direction, speed, center);
}

void Up() {
    double speed = 0.01;
    center[1] += speed;
}

void Down() {

    double speed = -0.01;
    center[1] += speed;

}


void moveForward() {
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;


}

void moveBack() {
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;

    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;
}


void finger(float x, float y, int base, int up) {
    glPushMatrix();
    glTranslatef(1.0, x, y);
    glRotatef((GLfloat) base, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.4, 0.2, 0.1);
    glutSolidCube(1);
    glPopMatrix();

    glTranslatef(0.2, 0.0, 0.0);
    glRotatef((GLfloat) up, 0.0, 0.0, 1.0);
    glTranslatef(0.2, 0.0, 0.0);
    glPushMatrix();
    glScalef(0.4, 0.2, 0.1);
    glutSolidCube(1);
    glPopMatrix();
    glPopMatrix();
}

void hand(int x, int y, int z,int n) {

    //glColor3f(0.5, 0.5, 0.2);
    glTranslatef(1.5, 1.275, 0);
    glScalef(0.5, 0.5, 0.4);
    glTranslatef(-1.0, 0.0, 0.0);
    if (n == 1)
        glRotatef((GLfloat) shoulder2_1, x, 0.0, 0);
    else
        glRotatef((GLfloat) shoulder2_2, x, 0.0, 0);
    glRotatef((GLfloat) shoulder, 0.0, z, 0.0);
    glRotatef(hand_up, 0, 0, 1);
    glRotatef(-90, 0, 0, 1);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.9, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef((GLfloat) elbow, 0.0, y, 0);
    glTranslatef(1.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(2.0, 0.9, 1.0);
    glutSolidCube(1.0);
    glPopMatrix();

}

void Leg(int leg, int knee, int out) {
    glPushMatrix();
    glTranslatef(-0.5, -1.5, 0.0);
    glRotatef(out, 0, 0, 1);
    glRotatef(leg, 1.0, 0.0, 0);
    glTranslatef(0, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.5, 1, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();
    glTranslatef(0, -0.5, 0.0);
    glRotatef(knee, 1.0, 0.0, 0);
    glTranslatef(0, -0.5, 0.0);
    glPushMatrix();
    glScalef(0.5, 1, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();
    glTranslatef(0, -0.65, 0.0);
    glPushMatrix();
    glScalef(0.5, 0.3, 0.8);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

//2
GLuint loadTexture(Image *image) {
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
            //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

GLuint _textureId;

void initRendering(char floor[]) {
    Image *image = loadBMP(floor);
    _textureId = loadTexture(image);
    delete image;
    glEnable(GL_LIGHTING);
    // Flip light switch
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    // assign light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    // Material Properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_NORMALIZE);
    //Enable smooth shading
    glShadeModel(GL_SMOOTH);
    // Enable Depth buffer
    glEnable(GL_DEPTH_TEST);

}
//2


void drawmodel1(GLMmodel *pmodel, char path[]) {
    if (!pmodel) {
        pmodel = glmReadOBJ(path);

        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
        glmScale(pmodel, .15);
    }
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}


void main_menu(int value) {
    if (value == 1) {

        initRendering("floor2.bmp");
        glutPostRedisplay();
    }
    if (value == 2) {

        initRendering("floor3.bmp");
        glutPostRedisplay();
    }


}

void display(void) { //3
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // Clear Depth and Color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
    //materials properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glPushMatrix();
    glTranslatef(0, 0, -1);


    //floor
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.5, -0.25, 2);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(0.5, -0.25, 2);
    glTexCoord2f(5.0f, 20.0f);
    glVertex3f(0.5, -0.25, -2);
    glTexCoord2f(0.0f, 20.0f);
    glVertex3f(-0.5, -0.25, -2);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
    //3


    //////.....body....../////
    //
    glTranslatef(0, -0.1, 0);
    //
    glPushMatrix();
    glTranslatef(-0.2, Y, 0);
    glScalef(0.05, 0.05, 0.05);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glTranslatef(0, 0, 1);
    glRotatef(around, 0, 1, 0);
    glTranslatef(0, 0, -1);
    //////.....hand....../////
    glTranslatef(0, 0.5, 0);
    glPushMatrix();
    hand(1, 1, 1,1);
    finger(-0.35, 0, finger1Base, finger1Up);
    finger(0.35, 0, finger2Base, finger2Up);
    finger(0.35, -0.45, finger3Base, finger3Up);
    finger(0.35, 0.45, finger4Base, finger4Up);
    glPopMatrix();
    glPushMatrix();
    glRotatef(-180, 0, -1, 0);
    hand(-1, -1, -1,2);
    finger(-0.35, 0, finger1Base, finger1Up);
    finger(0.35, 0, finger2Base, finger2Up);
    finger(0.35, -0.45, finger3Base, finger3Up);
    finger(0.35, 0.45, finger4Base, finger4Up);
    glPopMatrix();
    //////.....head....../////
    glPushMatrix();
    glTranslatef(0, 2.25, 0);
    glutSolidSphere(0.5, 32, 32);
    glPopMatrix();
    glPushMatrix();
    glScalef(1.5, 3, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();
    /////........leg.......////
    glPushMatrix();
    Leg(rhip, rknee, rhip2);
    glPopMatrix();
    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    Leg(lhip, lknee, lhip2);
    glPopMatrix();
    glPopMatrix();
    //////.....body_end....../////
    //4

    //right bed draw
    glPushMatrix();
    glTranslatef(0.3, -0.05, 0.5);
    glScalef(1.3, 1.3, 1.4);

    glRotatef(90, 1, 0, 0);
    glRotatef(180, 0, 1, 0);
    // glScalef(.25, .25, .25);
    drawmodel1(pmodel, "data/California_King_Size_Bed_With_Thyme_Sheets_Pine_V1_NEW.obj");
    glPopMatrix();

    //left bed draw
    glPushMatrix();
    glTranslatef(-0.3, -0.05, 0.5);
    glScalef(1.3, 1.3, 1.4);

    glRotatef(90, 1, 0, 0);
    glRotatef(180, 0, 1, 0);
    // glScalef(.25, .25, .25);
    drawmodel1(pmodel2, "data/California_King_Size_Bed_With_Thyme_Sheets_Pine_V1_NEW.obj");
    glPopMatrix();


    glPopMatrix();
//    glutSwapBuffers();

    //flowers draw
    glPushMatrix();
    glTranslatef(0.0, -0.12, -0.3);
    glScalef(1.5, 0.8, 1.4);

    glRotatef(0, 1, 0, 0);
    glRotatef(0, 0, 1, 0);
    // glScalef(.25, .25, .25);
    drawmodel1(pmodel3, "data/flowers.obj");
    glPopMatrix();

    //ball draw
    glPushMatrix();
    glTranslatef(0.0, -0.2, -0.95);
    glScalef(0.2, 0.2, 0.2);

    glRotatef(0, 1, 0, 0);
    glRotatef(0, 0, 1, 0);
    // glScalef(.25, .25, .25);
    drawmodel1(pmodel4, "data/soccerball.obj");
    glPopMatrix();

    //porsh draw
    glPushMatrix();
    glTranslatef(+0.32, -0.2, -1.05);
    glScalef(0.5, 0.5, 0.5);

    glRotatef(0, 1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    // glScalef(.25, .25, .25);
    drawmodel1(pmodel5, "data/porsche.obj");
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
    //4
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            Left();
            break;
        case GLUT_KEY_RIGHT:
            Right();
            break;
        case GLUT_KEY_UP:
            Up();
            break;
        case GLUT_KEY_DOWN:
            Down();
            break;
    }

    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {

        case 'f':
            moveForward();
            glutPostRedisplay();
            break;
        case 'b':
            moveBack();
            glutPostRedisplay();
            break;
            /////.....Assignment2 buttons..../////
        case 's':
            if (shoulder < 90) {
                shoulder = (shoulder + 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'S':
            if (shoulder > -90) {
                shoulder = (shoulder - 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'w':
            if (hand_up < 90) {
                hand_up = (hand_up + 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'W':
            if (hand_up > -90) {
                hand_up = (hand_up - 5) % 360;
                glutPostRedisplay();
            }
            break;


        case 'i':
            if (shoulder2_1 > -180) {
                shoulder2_1 = (shoulder2_1 - 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'I':
            if (shoulder2_1 < 30) {
                shoulder2_1 = (shoulder2_1 + 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'y':
            if (shoulder2_2 > -180) {
                shoulder2_2 = (shoulder2_2 - 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'Y':
            if (shoulder2_2 < 30) {
                shoulder2_2 = (shoulder2_2 + 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'E':
            if (elbow < 160) {
                elbow = (elbow + 5) % 360;
                glutPostRedisplay();
                break;
            }
        case 'e':
            if (elbow > -120) {
                elbow = (elbow - 5) % 360;
                glutPostRedisplay();
            }
            break;


        case 't':
            if (finger1Base < 90) {
                finger1Base = (finger1Base + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'T':
            if (finger1Base <= 90 && finger1Base > 0) {
                finger1Base = (finger1Base - 5) % 360;
                glutPostRedisplay();
            }
            break;


        case 'g':
            if (finger1Up < 90) {
                finger1Up = (finger1Up + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'G':
            if (finger1Up <= 90 && finger1Up > 0) {
                finger1Up = (finger1Up - 5) % 360;
                glutPostRedisplay();
            }
            break;

            //*******************************
        case 'z':
            if (finger2Base < 90) {

                finger2Base = (finger2Base + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'Z':
            if (finger2Base <= 90 && finger2Base > 0) {

                finger2Base = (finger2Base - 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'x':
            if (finger2Up < 90) {

                finger2Up = (finger2Up + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'X':
            if (finger2Up <= 90 && finger2Up > 0) {

                finger2Up = (finger2Up - 5) % 360;
                glutPostRedisplay();
            }
            break;

            //*******************************
        case 'c':
            if (finger3Base < 90) {

                finger3Base = (finger3Base + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'C':
            if (finger3Base <= 90 && finger3Base > 0) {

                finger2Base = (finger3Base - 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'v':
            if (finger3Up < 90) {

                finger3Up = (finger3Up + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'V':
            if (finger3Up <= 90 && finger3Up > 0) {

                finger3Up = (finger3Up - 5) % 360;
                glutPostRedisplay();
            }
            break;

            //*******************************
        case 'a':
            if (finger4Base < 90) {

                finger4Base = (finger4Base + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'A':
            if (finger4Base <= 90 && finger4Base > 0) {

                finger4Base = (finger4Base - 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'n':
            if (finger4Up < 90) {

                finger4Up = (finger4Up + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'N':
            if (finger4Up <= 90 && finger4Up > 0) {

                finger4Up = (finger4Up - 5) % 360;
                glutPostRedisplay();
            }
            break;
            //*******************************
        case 'l':
            if (rhip < 90 ) {
                rhip = (rhip + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'L':
            if (rhip <= 90 ) {
                rhip = (rhip - 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'P':
            if (lhip < 90) {
                lhip = (lhip + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'p':
            if (lhip <= 90) {
                lhip = (lhip - 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'o':
            if (rknee < 90 ) {
                rknee = (rknee + 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'O':
            if (rknee <= 90 ) {
                rknee = (rknee - 5) % 360;
                glutPostRedisplay();
            }

            break;
        case 'u':
            if (lknee > -90 ) {
                lknee = (lknee - 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'U':
            if (lknee >= -90) {
                lknee = (lknee + 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'q':
            if (rhip2 > -90 && lhip == 0 && lknee == 0 && lhip2 == 0) {
                rhip2 = (rhip2 - 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'Q':
            if (rhip2 >= -90 && rhip2 <= 0 && lhip == 0 && lknee == 0 && lhip2 == 0) {
                rhip2 = (rhip2 + 5) % 360;
                glutPostRedisplay();
            }
            break;

        case 'm':
            if (lhip2 > -90 && rhip == 0 && rknee == 0 && rhip2 == 0) {
                lhip2 = (lhip2 - 5) % 360;
                glutPostRedisplay();
            }
            break;
        case 'M':
            if (lhip2 >= -90 && lhip2 <= 0 && rhip == 0 && rknee == 0 && rhip2 == 0) {
                lhip2 = (lhip2 + 5) % 360;
                glutPostRedisplay();
            }
            break;

        case '0':

            around = (around - 5) % 360;
            glutPostRedisplay();
            break;
        case '1':


            around = (around + 5) % 360;
            glutPostRedisplay();
            break;

        case '2':
            glutTimerFunc(150, walk, 0);
            break;

        case '3':
            glutTimerFunc(150, jump, 0);
            break;

        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

static void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {

        if (state == GLUT_DOWN) {
            Right();
            moving = 1;
            glutPostRedisplay();
        }
        if (state == GLUT_UP) {
            moving = 0;

        }
    }
    if (button == GLUT_RIGHT_BUTTON) {

        if (state == GLUT_DOWN) {
            Left();
            moving = 2;
            glutPostRedisplay();
        }
        if (state == GLUT_UP) {
            moving = 0;

        }
    }
}

static void motion(int x, int y) {
    if (moving == 1) {
        Right();
        glutPostRedisplay();
    }
    if (moving == 2) {
        Left();
        glutPostRedisplay();

    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    //5
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("body");
    initRendering("floor1.bmp");
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, aspect, 0.1, 10);
    glutCreateMenu(main_menu);
    glutAddMenuEntry("First_Texure", 1);
    glutAddMenuEntry("Second_Texure", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    init();
//    glutTimerFunc(0,timer,0);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);


    glutMainLoop();
    return 0;
}
