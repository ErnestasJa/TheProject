import bpy
import iqm_export

class linked_duplicate(bpy.types.Operator):
    bl_idname = "object.linked_duplicate"
    bl_label = "Linked duplicate"

    def execute(self, context):
        bpy.ops.object.duplicate(True)
        bpy.ops.object.transform.translate()
        return {'FINISHED'}

class IBS_Tools(bpy.types.Panel):
    """IBS toolkit"""
    bl_label = "IBS Toolkit"
    bl_space_type = "VIEW_3D"
    bl_region_type = "TOOLS"
    
    def draw(self, context):
        layout = self.layout
        
        row = layout.row()
        row.label(text="Scene tools")
        
        col = layout.column(align=True)
        col.operator('object.proxy_make', text = "Make proxy")
        col.operator("object.duplicate_move_linked", text = "Make linked dup")
        
        row = layout.row()
        row.label(text="Export scene")
        col = layout.column(align=True)
        col.operator("export_scene.ibs", text = "Export ibs")
        
def register():
    bpy.utils.register_class(IBS_Tools)
    bpy.utils.register_class(linked_duplicate)
    
def unregister():
    bpy.utils.unregister_class(IBS_Tools)
    bpy.utils.unregister_class(linked_duplicate)
    
if __name__ == "__main__":
    register()