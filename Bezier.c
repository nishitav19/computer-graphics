#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define PI 3.1416

GLsizei winWidth = 600, winHeight = 600;
GLfloat xwcMin = 0.0, xwcMax = 130.0;
GLfloat ywcMin = 0.0, ywcMax = 130.0;

int ch = 0;
struct wcPt3D {

    GLfloat x, y, z;
};

int id2;

void bino (GLint n, GLint *C) {

    GLint k, j;
    for(k = 0; k <= n; k++) {

        C[k] = 1;

        for(j = n; j >= k + 1; j--)
            C[k] *= j;

        for(j = n - k; j >= 2; j--)
            C[k] /= j;
    }
}

void computeBezPt(GLfloat u, wcPt3D *bezPt, GLint nCtrlPts, wcPt3D *ctrlPts, GLint *C) {

    GLint k, n = nCtrlPts - 1;
    GLfloat bezBlendFcn;
    bezPt -> x = bezPt -> y = bezPt -> z = 0.0;
    for(k = 0; k < nCtrlPts; k++) {

        bezBlendFcn = C[k] * pow(u, k) * pow(1 - u, n - k);
        bezPt -> x += ctrlPts[k].x * bezBlendFcn;
        bezPt -> y += ctrlPts[k].y * bezBlendFcn;
        bezPt -> z += ctrlPts[k].z * bezBlendFcn;
    }
}
void bezier(wcPt3D *ctrlPts, GLint nCtrlPts, GLint nBezCurvePts) {

    wcPt3D bezCurvePt;
    GLfloat u;
    GLint *C, k;
    C = new GLint[nCtrlPts];
    bino(nCtrlPts - 1, C);
    glBegin(GL_LINE_STRIP);
    for(k = 0; k <= nBezCurvePts; k++) {

        u = GLfloat(k) / GLfloat(nBezCurvePts);
        computeBezPt(u, &bezCurvePt, nCtrlPts, ctrlPts, C);
        glVertex2f(bezCurvePt.x, bezCurvePt.y);

    }
    glEnd();
    delete[]C;
}

void displayFcn() {

   if(ch == 0) {

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,0);
    }

    if(ch == 1 | ch == 2) {

        GLint nCtrlPts = 4, nBezCurvePts = 20;
        static float theta = 0;

        wcPt3D ctrlPts[4] = {{20, 100, 0}, {30, 110, 0}, {50, 90, 0}, {60, 100, 0}};

        if(id2 == 1) {

            ctrlPts[1].x += 10 * sin(theta * PI / 180.0);
            ctrlPts[1].y += 5 * sin(theta * PI / 180.0);

            ctrlPts[2].x -= 10 * sin((theta + 30) * PI / 180.0);
            ctrlPts[2].y -= 10 * sin((theta + 30) * PI / 180.0);

            ctrlPts[3].x -= 4 * sin(theta * PI / 180.0);
            ctrlPts[3].y += sin((theta - 30) * PI / 180);

            theta += 1;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glPointSize(5);

        glPushMatrix();
        glLineWidth(5);
        // Orange
        glColor3f(255 / 255, 153 / 255.0, 51 / 255.0);
        for(int i = 0; i < 8; i++) {

            glTranslatef(0, -0.8, 0);
            bezier(ctrlPts, nCtrlPts, nBezCurvePts);
        }

        // White
        glColor3f(1.0, 1.0, 1.0);
        for(int i = 0; i < 8; i++) {

            glTranslatef(0, -0.8, 0);
            bezier(ctrlPts, nCtrlPts, nBezCurvePts);
        }

        // Green
        glColor3f(19 / 255.0, 136 / 255.0, 8 / 255.0);
        for(int i = 0; i < 8; i++) {

            glTranslatef(0, -0.8, 0);
            bezier(ctrlPts, nCtrlPts, nBezCurvePts);
        }
        glPopMatrix();

        // Pole
        glColor3f(0.7, 0.5, 0.3);
        glLineWidth(5);
        glBegin(GL_LINES);
            glVertex2f(20, 100);
            glVertex2f(20, 40);
        glEnd();

        glFlush();
        glutPostRedisplay();
        glutSwapBuffers();
    }
}
void winReshape(GLint newWidth, GLint newHeight) {

    glViewport(0, 0, newWidth, newHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(xwcMin, xwcMax, ywcMin, ywcMax);
    glClear(GL_COLOR_BUFFER_BIT);

}
void menu(int id) {

    glClear(GL_COLOR_BUFFER_BIT);
    switch(id) {
        case 1: exit(0);
                break;

        case 2: ch = 1;
                id2 = 1;
                break;

        case 3: ch = 2;
                id2 = 2;
                break;
        glutPostRedisplay();
    }
    displayFcn();
}
int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Bezier Curve");
    glutCreateMenu(menu);
    glutDisplayFunc(displayFcn);
    glutAddMenuEntry("Exit", 1);
    glutAddMenuEntry("Draw Flag", 3);
    glutAddMenuEntry("Flag Animation", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutReshapeFunc(winReshape);
    glutMainLoop();
    return 0;
}

