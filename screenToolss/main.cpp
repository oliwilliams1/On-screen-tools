#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "screenshotTaker.h"
#include "Object.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

Object *screenObject;
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

    screenObject->render();

    glutPostRedisplay();
    glutSwapBuffers();
}

static int glfwThing(std::vector<uint8_t> imageData)
{
    if (!glewInit() == GLEW_OK) return 1;

    std::vector<vec3> vertices = { vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0), vec3(1, 0, 0) };
    std::vector<vec3> indices  = { vec3(0, 1, 2), vec3(0, 2, 3) };
    std::vector<vec2> texCoords= { vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 0) };

    imageType imageType;
    imageType.width = 800;
    imageType.height = 600;
    Object tempObject = Object(vertices, indices, texCoords, imageData, imageType);
    screenObject = &tempObject;

    glutDisplayFunc(renderCB);

    glutMainLoop();

    return 0;
}

int main() {
    ScreenshotTaker* screenshotTaker = new ScreenshotTaker();
    std::vector<uint8_t> imageData = screenshotTaker->takeScreenshot();

    if (!init()) return 1;

    glfwThing(imageData);

    if (screenshotTaker != NULL) {
        delete screenshotTaker;
    }

    close();

    return 0;
}