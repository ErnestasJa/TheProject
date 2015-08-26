#include "Precomp.h"
#include "OctreeUtils.h"
#include <Python.h>
#include "motree/Morton.h"

PyObject * EncodeMKPy(PyObject* self, PyObject* arg)
{
    uint32_t x,y,z,mk;

    if(PyArg_ParseTuple(arg, "iii", &x,&y,&z))
    {
        mk = encodeMK(x,y,z);
        return PyLong_FromLong(mk);
    }

    return Py_None;
}

PyObject * DecodeMKPy(PyObject* self, PyObject* arg)
{
    uint32_t x,y,z,mk;

    if(PyArg_ParseTuple(arg, "i", &mk))
    {
        decodeMK(mk, x, y, z);
        return Py_BuildValue("iii", x,y,z);
    }

    return Py_None;
}

// Register the wrapped functions.
static PyMethodDef OctreeMethods[] =
{
    {"encode_mk", EncodeMKPy, METH_VARARGS, "Encode x,y,z coordinates into morton key."},
    {"decode_mk", DecodeMKPy, METH_VARARGS, "Decode morton key into x,y,z coordinates."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef OctreeModule = {
   PyModuleDef_HEAD_INIT,
   "Octree",   /* name of module */
   "Octree Utilities", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   OctreeMethods
};

PyMODINIT_FUNC PyInit_Octree(void)
{
    PyObject *module = PyModule_Create(&OctreeModule);

    return  module;
}
