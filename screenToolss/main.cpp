#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "screenshotTaker.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

GLuint texture;

static bool init()
{
    int argc = 0;
    char* argv[] = { NULL };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    int window = glutCreateWindow("hello_glfw");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "could not initialize glew\n");
        return false;
    }

    return true;
}

static void close()
{
    glutDestroyWindow(glutGetWindow());
    glutLeaveMainLoop();
}

static void renderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /*/ Render the texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw a full-screen quad to display the texture
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    */

    glutPostRedisplay();
    glutSwapBuffers();
}

static int glfwThing(std::vector<uint8_t> imageData)
{
    if (!glewInit() == GLEW_OK) return 1;

    /*GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    */

    glutDisplayFunc(renderCB);

    glutMainLoop();

    return 0;
}

int main() {
    if (!init()) return 1;

    ScreenshotTaker* screenshotTaker = new ScreenshotTaker();
    std::vector<uint8_t> imageData = screenshotTaker->takeScreenshot();

    glfwThing(imageData);

    if (screenshotTaker != NULL) {
        delete screenshotTaker;
    }

    close();

    return 0;
}