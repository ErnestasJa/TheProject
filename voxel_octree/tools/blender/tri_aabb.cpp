/********************************************************/

/* AABB-triangle overlap test code                      */

/* by Tomas Akenine-Möller                              */

/* Function: int triBoxOverlap(float boxcenter[3],      */

/*          float boxhalfsize[3],float triverts[3][3]); */

/* History:                                             */

/*   2001-03-05: released the code in its first version */

/*   2001-06-18: changed the order of the tests, faster */

/*                                                      */

/* Acknowledgement: Many thanks to Pierre Terdiman for  */

/* suggestions and discussions on how to optimize code. */

/* Thanks to David Hunt for finding a ">="-bug!         */

/********************************************************/

#include <math.h>

#include <stdio.h>



#define X 0

#define Y 1

#define Z 2



#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 



#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])



#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 



#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;



int planeBoxOverlap(float normal[3], float vert[3], float maxbox[3])	// -NJMP-

{

  int q;

  float vmin[3],vmax[3],v;

  for(q=X;q<=Z;q++)

  {

    v=vert[q];					// -NJMP-

    if(normal[q]>0.0f)

    {

      vmin[q]=-maxbox[q] - v;	// -NJMP-

      vmax[q]= maxbox[q] - v;	// -NJMP-

    }

    else

    {

      vmin[q]= maxbox[q] - v;	// -NJMP-

      vmax[q]=-maxbox[q] - v;	// -NJMP-

    }

  }

  if(DOT(normal,vmin)>0.0f) return 0;	// -NJMP-

  if(DOT(normal,vmax)>=0.0f) return 1;	// -NJMP-

  

  return 0;

}





/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;



#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;



/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;



#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return 0;


/*======================== Z-tests ========================*/



#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;



#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return 0;



int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3])

{



  /*    use separating axis theorem to test overlap between triangle and box */

  /*    need to test for overlap in these directions: */

  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */

  /*       we do not even need to test these) */

  /*    2) normal of the triangle */

  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */

  /*       this gives 3x3=9 more tests */

   float v0[3],v1[3],v2[3];

//   float axis[3];

   float min,max,p0,p1,p2,rad,fex,fey,fez;		// -NJMP- "d" local variable removed

   float normal[3],e0[3],e1[3],e2[3];



   /* This is the fastest branch on Sun */

   /* move everything so that the boxcenter is in (0,0,0) */

   SUB(v0,triverts[0],boxcenter);

   SUB(v1,triverts[1],boxcenter);

   SUB(v2,triverts[2],boxcenter);



   /* compute triangle edges */

   SUB(e0,v1,v0);      /* tri edge 0 */

   SUB(e1,v2,v1);      /* tri edge 1 */

   SUB(e2,v0,v2);      /* tri edge 2 */



   /* Bullet 3:  */

   /*  test the 9 tests first (this was faster) */

   fex = fabsf(e0[X]);

   fey = fabsf(e0[Y]);

   fez = fabsf(e0[Z]);

   AXISTEST_X01(e0[Z], e0[Y], fez, fey);

   AXISTEST_Y02(e0[Z], e0[X], fez, fex);

   AXISTEST_Z12(e0[Y], e0[X], fey, fex);



   fex = fabsf(e1[X]);

   fey = fabsf(e1[Y]);

   fez = fabsf(e1[Z]);

   AXISTEST_X01(e1[Z], e1[Y], fez, fey);

   AXISTEST_Y02(e1[Z], e1[X], fez, fex);

   AXISTEST_Z0(e1[Y], e1[X], fey, fex);



   fex = fabsf(e2[X]);

   fey = fabsf(e2[Y]);

   fez = fabsf(e2[Z]);

   AXISTEST_X2(e2[Z], e2[Y], fez, fey);

   AXISTEST_Y1(e2[Z], e2[X], fez, fex);

   AXISTEST_Z12(e2[Y], e2[X], fey, fex);



   /* Bullet 1: */

   /*  first test overlap in the {x,y,z}-directions */

   /*  find min, max of the triangle each direction, and test for overlap in */

   /*  that direction -- this is equivalent to testing a minimal AABB around */

   /*  the triangle against the AABB */



   /* test in X-direction */

   FINDMINMAX(v0[X],v1[X],v2[X],min,max);

   if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return 0;



   /* test in Y-direction */

   FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);

   if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return 0;



   /* test in Z-direction */

   FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);

   if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return 0;



   /* Bullet 2: */

   /*  test if the box intersects the plane of the triangle */

   /*  compute plane equation of triangle: normal*x+d=0 */

   CROSS(normal,e0,e1);

   // -NJMP- (line removed here)

   if(!planeBoxOverlap(normal,v0,boxhalfsize)) return 0;	// -NJMP-



   return 1;   /* box and triangle overlaps */

}

#include <stdint.h>
#include <vector>

template <class T>
struct vec3 {

  union{
    T vec[3];
    struct {T x,y,z;};
  };

  vec3(){};

  vec3(T x, T y, T z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  void set(T *v){
    x=v[0];
    y=v[1];
    z=v[2];
  }

  void set(T a, T b, T c){
    x=a;
    y=b;
    z=c;
  }

  T & operator [] (int32_t index){
    return this->vec[index];
  }
};

namespace util
{
  template <class T>
  T min(const T & a, const T & b)
  {
    return a<b?a:b;
  }

  template <class T>
  T max(const T & a, const T & b)
  {
    return a>b?a:b;
  }
}

struct Triangle{
  vec3<float> min, max;
  vec3<float> center, halfsize, a, b ,c;

  void set(float * triangle)
  {
    min.x=util::min(util::min(triangle[0],triangle[3]),triangle[6]);
    min.y=util::min(util::min(triangle[1],triangle[4]),triangle[7]);
    min.z=util::min(util::min(triangle[2],triangle[5]),triangle[8]);

    max.x=util::max(util::max(triangle[0],triangle[3]),triangle[6]);
    max.y=util::max(util::max(triangle[1],triangle[4]),triangle[7]);
    max.z=util::max(util::max(triangle[2],triangle[5]),triangle[8]);

    halfsize.x = (max.x-min.x)*0.5f;
    halfsize.y = (max.y-min.y)*0.5f;
    halfsize.z = (max.z-min.z)*0.5f;

    center.x = min.x + halfsize.x;
    center.y = min.y + halfsize.y;
    center.z = min.z + halfsize.z;

    a.set(triangle);
    b.set(triangle+3);
    c.set(triangle+6);
  }

  void get(float out[3][3]){
    out[0][0]=a.x;
    out[0][1]=a.y;
    out[0][2]=a.z;

    out[1][0]=b.x;
    out[1][1]=b.y;
    out[1][2]=b.z;

    out[2][0]=c.x;
    out[2][1]=c.y;
    out[2][2]=c.z;
  }
};


struct Voxel {
  vec3<uint16_t> pos;
};


void triangle_output(Triangle & tri, std::vector<Voxel> & voxels){
  vec3<float> hs, center;
  hs.set(0.5f,0.5f,0.5f);

  float triangle[3][3];
  tri.get(triangle);
  Voxel v;

  for(uint16_t z = tri.min.z; z < (int)(tri.max.z+1); z++)
    for(uint16_t y = tri.min.y; y < (int)(tri.max.y+1); y++)
      for(uint16_t x = tri.min.x; x < (int)(tri.max.x+1); x++)
      {
        center.set(x+0.5f,y+0.5f,z+0.5f);
        if(triBoxOverlap(center.vec, hs.vec, triangle))
        {
          v.pos = vec3<uint16_t>(x,y,z);
          voxels.push_back(v);
        }
      }
}


void write_voxels(const char * filename, std::vector<Voxel> & voxels);
void voxel_output(const char * filename, float * triangles, uint32_t triangle_count)
{
  float * triangle = triangles; 
  Triangle tri;
  std::vector<Voxel> voxels;

  for(uint32_t i = 0; i < triangle_count ; i++)
  {
    tri.set(triangle);
    triangle_output(tri, voxels);
    triangle+=9;
  }

  write_voxels(filename, voxels);
}

#include <fstream>
void write_voxels(const char * filename, std::vector<Voxel> & voxels)
{
  std::ofstream file(filename, std::ios::out | std::ios::binary);

  printf("Trying to write %i voxels to file\n", voxels.size());

  uint32_t count = voxels.size();
  if(voxels.size()>0)
  {
    file.write((char*)&count, 4);

    for(uint32_t i = 0; i < count; i++){
      file.write((char*)&voxels[i].pos.x, 2);
      file.write((char*)&voxels[i].pos.y, 2);
      file.write((char*)&voxels[i].pos.z, 2);
    }
  }

  file.close();
}

#include <Python.h>

struct module_state {
    PyObject *error;
};

#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#define INITERROR return NULL

static struct module_state _state;
 
static PyObject* tri_aabb_test(PyObject* self, PyObject* args)
{
    float boxcenter[3], boxhalfsize[3], triverts[3][3];
 
    if (!PyArg_ParseTuple(args, "fffffffffffffff", &boxcenter[0], &boxcenter[1], &boxcenter[2],
       &boxhalfsize[0], &boxhalfsize[1], &boxhalfsize[2],
	     &triverts[0][0], &triverts[0][1], &triverts[0][2],
	     &triverts[1][0], &triverts[1][1], &triverts[1][2],
	     &triverts[2][0], &triverts[2][1], &triverts[2][2]))
        return NULL;

    return Py_BuildValue("i", triBoxOverlap(boxcenter, boxhalfsize, triverts));
}

static PyObject* voxelize(PyObject* self, PyObject* args)
{
    const char * filename;
    int count;
    PyObject * list;
 
    if (!PyArg_ParseTuple(args, "siO", &filename, &count, &list)){
      printf("Failed to parse function args\n");
      return NULL;
    }

    uint32_t len = PyList_Size(list);
    Py_INCREF(list);

    if(len<=0 || len%9!=0){
      printf("Triangle list is incorrectly formated or empty\n");
      return NULL;
    }

    printf("%s %u\n", "Got list of size: ", len);

    float *triangles = new float[len];

    for(uint32_t i = 0; i < len/9; i++){
      for(uint32_t j = 0; j < 9; j++)
      {
        uint32_t index = (i*9) + j;
        PyObject * item = PyList_GetItem(list, index);

        if(item==NULL){
          printf("%s\n", "List item is null");
          return NULL;
        }

        Py_INCREF(item);
        triangles[index] = PyFloat_AsDouble(item);
        Py_DECREF(item);
      }
    }

    Py_DECREF(list);

    voxel_output(filename, triangles, len/9);

    return Py_BuildValue("i", 1);
}
 
static PyMethodDef tri_aabb_methods[] =
{
     {"tri_aabb_test", tri_aabb_test, METH_VARARGS, "Tests tri aabb."},
     {"voxelize", voxelize, METH_VARARGS, "Voxelizes triangle mesh and outputs to file."},
     {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "tri_aabb",
        NULL,
        sizeof(struct module_state),
        tri_aabb_methods,
        NULL,
        NULL,
        NULL,
        NULL
};
 
PyMODINIT_FUNC PyInit_tri_aabb(void)
{
    PyObject *module = PyModule_Create(&moduledef);

     if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("myextension.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

    return module;
}
                                                                                                                                                                                            