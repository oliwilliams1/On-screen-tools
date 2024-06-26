#include "ScreenshotTaker.h"

void ScreenshotTaker::initialize()
{
    // Initialize the Python interpreter
    Py_Initialize();

    // Import the ImageGrab module
    PyObject* pName = PyUnicode_FromString("PIL.ImageGrab");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule == NULL) {
        std::cerr << "Failed to import module" << std::endl;
        Py_Finalize();
        return;
    }

    // Get a reference to the grab() function
    pFunc = PyObject_GetAttrString(pModule, "grab");
    if (pFunc == NULL || !PyCallable_Check(pFunc)) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cerr << "Cannot find function 'grab'" << std::endl;
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        Py_Finalize();
        return;
    }
}

ScreenshotTaker::ScreenshotTaker()
{
    initialize();
}

ScreenshotTaker::~ScreenshotTaker()
{
    Py_XDECREF(pSaveFunc);
    Py_XDECREF(pScreenshot);
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}

void ScreenshotTaker::takeScreenshot(const std::string& filename)
{
    pScreenshot = PyObject_CallObject(pFunc, NULL);
    if (pScreenshot == NULL) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cerr << "Error occurred while taking screenshot" << std::endl;
        return;
    }

    pSaveFunc = PyObject_GetAttrString(pScreenshot, "save");
    if (pSaveFunc && PyCallable_Check(pSaveFunc)) {
        PyObject* pFilename = PyUnicode_FromString(filename.c_str());
        PyObject_CallFunction(pSaveFunc, "O", pFilename);
        Py_DECREF(pFilename);
    }
    else {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cerr << "Error occurred while saving screenshot" << std::endl;
    }
    std::cout << "Screenshot saved to " << filename << std::endl;
}