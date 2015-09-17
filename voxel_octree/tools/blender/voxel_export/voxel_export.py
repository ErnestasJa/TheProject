import bpy
from math import floor
import tri_aabb

vhs = 0.5 #voxel half size

def TestVoxel(pos, triVerts):
    return tri_aabb.tri_aabb_test(pos[0],pos[1],pos[2],vhs,vhs,vhs,
        triVerts[0][0],triVerts[0][1],triVerts[0][2],
        triVerts[1][0],triVerts[1][1],triVerts[1][2],
        triVerts[2][0],triVerts[2][1],triVerts[2][2])
        
class Tri:
    def __init__(self, A, B, C):
        self.verts = [A, B, C]
        self.min = [int(min(A[0],B[0],C[0])),int(min(A[1],B[1],C[1])),int(min(A[2],B[2],C[2]))]
        self.max = [int(max(A[0],B[0],C[0]))+1,int(max(A[1],B[1],C[1]))+1,int(max(A[2],B[2],C[2]))+1]
        
    def outputVoxels(self, list):
        for z in range(self.min[2],self.max[2]):
            for y in range(self.min[1],self.max[1]):
                for x in range(self.min[0],self.max[0]):
                    if TestVoxel([x+0.5,y+0.5,z+0.5],self.verts):
                        list.append([x,y,z])

from struct import *

def save(operator,
         context,
         filepath="",
         use_mesh_modifiers=True,
         use_normals=True,
         use_uv_coords=True,
         use_colors=True,
         global_matrix=None
         ):

    scene = context.scene
    obj = context.active_object

    if global_matrix is None:
        from mathutils import Matrix
        global_matrix = Matrix()

    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')

    if use_mesh_modifiers and obj.modifiers:
        mesh = obj.to_mesh(scene, False, 'PREVIEW')
    else:
        mesh = obj.data.copy()

    if not mesh:
        raise Exception("Error, could not get mesh data from active object")

    mesh.transform(global_matrix * obj.matrix_world)
    if use_normals:
        mesh.calc_normals()

    ret = voxel_export(filepath, mesh,
                    use_normals=use_normals,
                    use_uv_coords=use_uv_coords,
                    use_colors=use_colors,
                    )

    if use_mesh_modifiers:
        bpy.data.meshes.remove(mesh)

    return ret


def voxel_export(filepath,
              mesh,
              use_normals=True,
              use_uv_coords=True,
              use_colors=True):

    voxels = []
    
    cf = 0
    cfl = len(mesh.polygons)
    for face in mesh.polygons:
        fv = face.vertices
        
        if len(fv)==3:
            Tri(mesh.vertices[fv[0]].co, mesh.vertices[fv[1]].co, mesh.vertices[fv[2]].co).outputVoxels(voxels)
            
        print(str(cf) + "/" + str(cfl))
        cf += 1
        
    print("Voxel count: " + str(len(voxels)))
    
    file = open(filepath, 'wb')
    file.write(pack("I",len(voxels)))

    for v in voxels:
        file.write(pack("3I",v[0],v[1],v[2]))

    file.close()

    print('Exported to :' + filepath)

    return {'FINISHED'}