#include "App.h"
// Init the instance variable
App* App::instance = nullptr;

const char* App::loadShaderSource(const char* filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* contents = new char[static_cast<size_t>(file_size) + 1];

    file.read(contents, file_size);
    contents[static_cast<size_t>(file_size)] = '\0';

    file.close();
    return contents;
}
void App::mousePosCallback(int x, int y)
{
    instance->mousePos = vec2((float)x, (float)y);
}

void App::renderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate the delta time
    instance->currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f; // Elapsed time in seconds
    float deltaTime = instance->currentTime - instance->previousTime;
    instance->previousTime = instance->currentTime;

    instance->selectionWindow.update();

    for (const auto& object : instance->objects) {
        object->draw();
    }

    glutPostRedisplay();
    glutSwapBuffers();
}

void App::mouseCB(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        instance->selectionWindow.mouseDown();
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        instance->selectionWindow.mouseUp();
    }
}

template <typename T>
void App::addObject(const T& object)
{
    static_assert(std::is_base_of_v<baseObject, std::decay_t<T>>, "Object must inherit from basicObject");
    objects.emplace_back(std::make_unique<std::decay_t<T>>(object));
}

void App::initScene(snoutImage image)
{
    const vec2 rectVertices[] = {
        {-0.5f, -0.5f},
        {-0.5f,  0.5f},
        { 0.5f,  0.5f},
        { 0.5f, -0.5f}
    };
    const GLuint rectIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const vec2 rectUVs[] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f}
    };

    const char* rectVertexShaderSource = loadShaderSource("vert.glsl");
    const char* rectFragmentShaderSource = loadShaderSource("frag.glsl");

    const vec2 fullRectVertices[] = {
        {-1.0f, -1.0f},
        {-1.0f,  1.0f},
        { 1.0f,  1.0f},
        { 1.0f, -1.0f}
    };

    const char* fullRectVertexShaderSource = loadShaderSource("vert.glsl");
    const char* fullRectFragmentShaderSource = loadShaderSource("frag2.glsl");

    advancedObject screenObjects(fullRectVertices, 4, rectIndices, 6, rectUVs, 4, fullRectVertexShaderSource, fullRectFragmentShaderSource, image);
    addObject(screenObjects);

    fractalRect fractalRectObject(rectVertices, 4, rectIndices, 6, rectUVs, 4, rectVertexShaderSource, rectFragmentShaderSource, &currentTime);
    addObject(fractalRectObject);

    const vec2 smallRectVertices[] = {
        { 0.0f,  0.0f},
        { 0.0f,  0.1f},
        { 0.1f,  0.1f},
        { 0.1f,  0.0f}
    };

    const char* smallRectVertexShaderSource = loadShaderSource("vert2.glsl");
    const char* smallRectFragmentShaderSource = loadShaderSource("frag3.glsl");

    const vec2 drawRectVertices[] = {
        { 0.0f,  0.0f},
        { 0.0f, -1.0f},
        { 1.0f, -1.0f},
        { 1.0f,  0.0f}
    };

    drawRect smallRect(drawRectVertices, 4, rectIndices, 6, rectUVs, 4, smallRectVertexShaderSource, smallRectFragmentShaderSource);
    addObject(smallRect);

    auto& storedSmallRect = *dynamic_cast<drawRect*>(objects.back().get());
    selectionWindow = SelectionWindow(&storedSmallRect, &image, &mousePos, &windowSize);
}

App::App(int* argc, char** argv)
{   
    if (instance == nullptr) {
        // Make the instance this so variables in this class can be acssessed from static callbacks
        instance = this;
    }
    ScreenshotTaker screenshotTaker;
    std::vector<uint8_t> imageData = screenshotTaker.takeScreenshot();

    snoutImage image = snoutImage(1920, 1080, 3, imageData);

    glutInit(argc, argv);
    if (debug) {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    }
    else {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_BORDERLESS);
    }

    glutInitWindowSize((int)windowSize.x, (int)windowSize.y);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example");

    glutPassiveMotionFunc(mousePosCallback);
    glutMotionFunc(mousePosCallback);

    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    initScene(image);
}

void App::run()
{
    glutDisplayFunc(renderCB);
    glutMouseFunc(mouseCB);
    glutMainLoop();
}