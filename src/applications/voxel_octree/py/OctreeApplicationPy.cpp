#include "precomp.h"
#include "OctreeApplicationPy.h"
#include "applications/voxel_octree/VoxelOctreeApp.h"


PyObject * LoadLevel(PyObject* self, PyObject* args)
{
    const char *str;

    if (!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    VoxelOctreeApp::Instance()->LoadLevel(str);

    Py_XDECREF(str);

    return Py_None;
}


// Register the wrapped functions.
static PyMethodDef OctreeApplicationMethods[] =
{
    {"load_level", LoadLevel, METH_VARARGS, "Load bvox level"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef OctreeApplicationModule = {
   PyModuleDef_HEAD_INIT,
   "OctreeApplication",   /* name of module */
   "Octree application module, use with great caution!!!", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   OctreeApplicationMethods//OctreeMethods
};

PyMODINIT_FUNC PyInit_OctreeApplication(void)
{
    PyObject *module = PyModule_Create(&OctreeApplicationModule);
    return  module;
}
