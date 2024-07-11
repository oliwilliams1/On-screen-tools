#include "App.h"
// Init the instance variable
App* App::instance = nullptr;

vec2 App::toNDC(vec2 value)
{
    return vec2(value.x * 2 - 1, value.y * 2 - 1);
}

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
    // Get mouse coords between 0 to 1
    instance->mousePos = vec2((x / instance->windowSize.x), (y / instance->windowSize.y));

    // To NDC
    instance->mousePos = instance->toNDC(instance->mousePos);

    // Idk why it works but it does
    instance->mousePos.y += 1.0f;
}

void App::renderCB()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Calculate the delta time
    instance->currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f; // Elapsed time in seconds
    float deltaTime = instance->currentTime - instance->previousTime;
    instance->previousTime = instance->currentTime;

    instance->selectionBox->update();

    for (const auto& object : instance->objects) {
        object->draw();
    }

    glutPostRedisplay();
    glutSwapBuffers();
}

void App::mouseCB(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        instance->mouseDown = true;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        instance->mouseDown = false;
    }
}

template <typename T>
void App::addObject(const T& object)
{
    static_assert(std::is_base_of_v<baseObject, std::decay_t<T>>, "Object must inherit from basicObject");
    objects.emplace_back(std::make_unique<std::decay_t<T>>(object));
}

void App::initScene(std::vector<uint8_t> imageData)
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

    advancedObject screenObjects(fullRectVertices, 4, rectIndices, 6, rectUVs, 4, fullRectVertexShaderSource, fullRectFragmentShaderSource, imageData, imageType(1920, 1080));
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

    selectionBox = new SelectionBox(&smallRect, &mousePos, &mouseDown);
}

App::App(int* argc, char** argv)
{
    if (instance == nullptr) {
        // Make the instance this so variables in this class can be acssessed from static callbacks
        instance = this;
    }
    ScreenshotTaker screenshotTaker;
    std::vector<uint8_t> imageData = screenshotTaker.takeScreenshot();

    glutInit(argc, argv);
    if (debug) {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    }
    else {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_BORDERLESS);
    }

    glutInitWindowSize(windowSize.x, windowSize.y);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL Example");

    glutPassiveMotionFunc(mousePosCallback);
    glutMotionFunc(mousePosCallback);

    glewInit();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    initScene(imageData);
}

void App::run()
{
    glutDisplayFunc(renderCB);
    glutMouseFunc(mouseCB);
    glutMainLoop();
}