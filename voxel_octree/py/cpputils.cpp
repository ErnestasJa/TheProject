#include "Precomp.h"
#include "cpputils.h"
#include "Python.h"
#include <iostream>

PyObject * Wrap_cout(PyObject* self, PyObject* arg)
{
    const char *str;

    if (!PyArg_ParseTuple(arg, "s", &str))
        return NULL;

    std::cout << str;
    
    Py_RETURN_NONE;
}

// Register the wrapped functions.
static PyMethodDef CppUtilsMethods[] =
{
    {"cout", Wrap_cout, METH_VARARGS, "Outputs string using standard console output"},
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
