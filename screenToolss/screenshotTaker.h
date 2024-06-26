#pragma once
#include <iostream>
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

class ScreenshotTaker
{
private:
    PyObject* pModule;
    PyObject* pFunc;
    PyObject* pScreenshot;
    PyObject* pSaveFunc;

    void initialize();

public:
    ScreenshotTaker();
    ~ScreenshotTaker();

    void takeScreenshot(const std::string& filename);
};