#include "Precomp.h"
#include "OctreeUtils.h"
#include <Python.h>
#include "motree/Morton.h"
#include "motree/MortonOctree.h"
#include "VoxelOctreeApp.h"

PyObject * EncodeMKPy(PyObject* self, PyObject* arg)
{
    uint32_t x,y,z,mk;

    if(PyArg_ParseTuple(arg, "iii", &x,&y,&z))
    {
        mk = encodeMK(x,y,z);
        return PyLong_FromLong(mk);
    }

    Py_RETURN_NONE;
}

PyObject * DecodeMKPy(PyObject* self, PyObject* arg)
{
    uint32_t x,y,z,mk;

    if(PyArg_ParseTuple(arg, "i", &mk))
    {
        decodeMK(mk, x, y, z);
        return Py_BuildValue("iii", x,y,z);
    }

    Py_RETURN_NONE;
}

///FIXME: Haaax :)
static uint8_t vox_r=255, vox_g=255, vox_b=255;

PyObject * AddVoxel(PyObject* self, PyObject* arg)
{
    uint32_t x,y,z;


    if(PyArg_ParseTuple(arg, "iii", &x,&y,&z))
    {
        VoxelOctreeApp::Instance()->GetOctree()->AddOrphanNode(MNode(encodeMK(x,y,z),1, vox_r, vox_g, vox_b));
    }

    Py_RETURN_NONE;
}


PyObject * SetVoxelColor(PyObject* self, PyObject* arg)
{
    int32_t r, g, b;


    if(PyArg_ParseTuple(arg, "iii", &r,&g,&b))
    {
        vox_r=r;
        vox_g=g;
        vox_b=b;
    }

    Py_RETURN_NONE;
}


PyObject * ClearOctree(PyObject* self, PyObject* arg)
{
    VoxelOctreeApp::Instance()->ClearOctree();

    Py_RETURN_NONE;
}

PyObject * GenerateOctreeMeshes(PyObject* self, PyObject* arg)
{
    VoxelOctreeApp::Instance()->GenerateOctreeMeshes();
    Py_RETURN_NONE;
}

PyObject * SetPlayerPosition(PyObject* self, PyObject* arg)
{
    float x,y,z;

    if(PyArg_ParseTuple(arg, "fff", &x,&y,&z))
    {
        VoxelOctreeApp::Instance()->SetPlayerPosition(x,y,z);
    }

    Py_RETURN_NONE;
}

// Register the wrapped functions.
static PyMethodDef OctreeMethods[] =
{
    {"encode_mk", EncodeMKPy, METH_VARARGS, "Encode x,y,z coordinates into morton key."},
    {"decode_mk", DecodeMKPy, METH_VARARGS, "Decode morton key into x,y,z coordinates."},
    {"add_voxel", AddVoxel, METH_VARARGS, "Adds a voxel to x,y,z location."},
    {"set_voxel_color", SetVoxelColor, METH_VARARGS, "Sets a voxel color to r,g,b values [0-255]."},
    {"clear_octree", ClearOctree, METH_NOARGS, "Adds a voxel to x,y,z location."},
    {"generate_octree_meshes", GenerateOctreeMeshes, METH_NOARGS, "Generate mesh from octree data."},
    {"set_player_position", SetPlayerPosition, METH_VARARGS, "Sets player position to x,y,z."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef OctreeModule = {
   PyModuleDef_HEAD_INIT,
   "octree",   /* name of module */
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
