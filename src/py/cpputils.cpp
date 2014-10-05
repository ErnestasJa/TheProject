#include "precomp.h"
#include "cpputils.h"
#include "python.h"
#include <iostream>

PyObject * Wrap_cout(PyObject* self, PyObject* arg)
{
    PyObject* pyStr = PyUnicode_AsEncodedString(arg, "utf-8", "Error ~");
    const char *str =  PyBytes_AS_STRING(pyStr);

    std::cout << str;

    Py_XDECREF(arg);
    Py_XDECREF(pyStr);

    return Py_None;
}

// Register the wrapped functions.
static PyMethodDef CppUtilsMethods[] =
{
    {"cout", Wrap_cout, METH_O, "Outputs string using standard console output"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef CppUtilsModule = {
   PyModuleDef_HEAD_INIT,
   "cpputils",   /* name of module */
   "C++ Utilities", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   CppUtilsMethods
};

PyMODINIT_FUNC PyInit_CppUtils(void)
{
    PyObject *module = PyModule_Create(&CppUtilsModule);

    return  module;
}
