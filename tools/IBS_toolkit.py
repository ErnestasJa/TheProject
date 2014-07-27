import bpy
import iqm_export
import imp
import sys

bl_info = {
    "name": "Inter blender scene (IBS) toolkit",
    "blender": (2, 69, 0),
    "location": "VIEW_3D",
    "description": "Tools to improve scene manipulation?!",
    "warning": "",
    "tracker_url": "",
    "category": "Object"}
	


def register_everything():
	my_path = bpy.path.abspath('//')

	if my_path not in sys.path:
		sys.path.insert(0, my_path)
		
	for module in sys.modules.values():
		if my_path in getattr(module, '__file__', ''):
			print('Reloading %s from %s' % (module, module.__file__))
			imp.reload(module)

class RegisterEverythingOperator(bpy.types.Operator):
	bl_idname = 'script.distance_register_everything'
	bl_label = 'Reload code'
	bl_description = 'Reloads all distance code.'

	def execute(self, context):
		register_everything()
		return {'FINISHED'}

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
	bl_category = "IBS"
	bl_space_type = "VIEW_3D"
	bl_region_type = "TOOLS"
    
	def draw(self, context):
		layout = self.layout
        
		row = layout.row()
		row.label(text="Scene tools")
		
		col = layout.column(align=True)
		col.operator('object.drop_on_active', text = "Drop to ground")
		col.operator('object.proxy_make', text = "Make proxy")
		col.operator("object.duplicate_move_linked", text = "Make linked dup")
        
		row = layout.row()
		row.label(text="Export scene")
		col = layout.column(align=True)
		col.operator("export_scene.ibs", text = "Export ibs")
		col.operator('script.distance_register_everything')
        
		
def register():
	bpy.utils.register_class(IBS_Tools)
	bpy.utils.register_class(linked_duplicate)
	bpy.utils.register_class(RegisterEverythingOperator)
	
    
def unregister():
	bpy.utils.unregister_class(IBS_Tools)
	bpy.utils.unregister_class(linked_duplicate)
	bpy.utils.unregister_class(RegisterEverythingOperator)
    
if __name__ == "__main__":
	register()
