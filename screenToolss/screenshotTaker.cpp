#include "ScreenshotTaker.h"
#include <iostream>

void ScreenshotTaker::initialize()
{
    Py_Initialize();

    // Import PIL
    PyObject* pName = PyUnicode_FromString("PIL.ImageGrab");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule == NULL) {
        std::cerr << "Failed to import module" << std::endl;
        Py_Finalize();
        return;
    }

    // Get grab function
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
    Py_XDECREF(pScreenshot);
    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
    Py_Finalize();
}

std::vector<uint8_t> ScreenshotTaker::takeScreenshot()
{
    pScreenshot = PyObject_CallObject(pFunc, NULL);
    if (pScreenshot == NULL) {
        if (PyErr_Occurred()) {
            PyErr_Print();
        }
        std::cerr << "Error occurred while taking screenshot" << std::endl;
        return std::vector<uint8_t>();
    }

    Py_INCREF(pScreenshot);

    // Get the byte data from the pScreenshot object
    PyObject* pByteData = PyObject_CallMethod(pScreenshot, "tobytes", NULL);
    if (pByteData == NULL) {
        std::cerr << "Error occurred while getting byte data from pScreenshot object" << std::endl;
        Py_DECREF(pScreenshot);
        return std::vector<uint8_t>();
    }

    // Copy the byte data into something c++ can use
    Py_ssize_t size = PyBytes_Size(pByteData);
    std::vector<uint8_t> imageData(size);
    memcpy(imageData.data(), PyBytes_AsString(pByteData), size);

    Py_DECREF(pByteData);
    Py_DECREF(pScreenshot);
    return imageData;
}