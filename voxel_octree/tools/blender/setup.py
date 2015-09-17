from distutils.core import setup, Extension
 
module1 = Extension('tri_aabb', sources = ['tri_aabb.cpp'])
 
setup (name = 'TriAABB',
        version = '1.0',
        description = 'This is a fully featured voxelizer package!',
	ext_modules = [module1] )