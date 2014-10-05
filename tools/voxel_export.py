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

class ExportSomeData(bpy.types.Operator):
    """Test exporter which just writes hello world"""
    bl_idname = "export.some_data"
    bl_label = "Export Some Data"
    
    filepath = bpy.props.StringProperty(subtype="FILE_PATH")

    @classmethod
    def poll(cls, context):
        return context.object is not None

    def execute(self, context):
        from mathutils import Matrix
        global_matrix = Matrix()
        
        obj = bpy.context.scene.objects.active
        mesh = obj.to_mesh(bpy.context.scene, False, "PREVIEW") # active object
        mesh.transform(global_matrix * obj.matrix_world)

        voxels = []
        
        cf = 0
        cfl = len(mesh.polygons)
        for face in mesh.polygons:
            fv = face.vertices
            
            if len(fv)==3:
                Tri(mesh.vertices[fv[0]].co, mesh.vertices[fv[1]].co, mesh.vertices[fv[2]].co).outputVoxels(voxels)
                
            print(str(cf) + "/" + str(cfl))
            cf += 1
            
        bpy.data.meshes.remove(mesh)
        print("Voxel count: " + str(len(voxels)))
        
        file = open(self.filepath, 'wb')
        file.write(pack("I",len(voxels)))

        for v in voxels:
            file.write(pack("3I",v[0],v[1],v[2]))

        file.close()
        
        return {'FINISHED'}

    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}


# Only needed if you want to add into a dynamic menu
def menu_func(self, context):
    self.layout.operator_context = 'INVOKE_DEFAULT'
    self.layout.operator(ExportSomeData.bl_idname, text="Voxel Export Operator")

# Register and add to the file selector
bpy.utils.register_class(ExportSomeData)
bpy.types.INFO_MT_file_export.append(menu_func)


# test call
bpy.ops.export.some_data('INVOKE_DEFAULT')
